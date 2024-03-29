#include <player.h>
#include <renderer.h>
#include <bullet.h>
#include <maths.h>

#define PLAYER_SPRITE_SIZE 15.f

#define PLAYER_ROT_SPEED 4.f
#define PLAYER_THRUST_BASE_SPEED 125.f
#define PLAYER_BRAKE_SPEED 2.f

#define PLAYER_CROSSHAIR_OFFSET 15.f

#define PLAYER_SHOOT_COOLDOWN_MS 300

#define PLAYER_POINT_0 (vec2f){ -5.f, 7.5f }
#define PLAYER_POINT_1 (vec2f){ 0.f , -7.5f }
#define PLAYER_POINT_2 (vec2f){ 5.f, 7.5f }
#define PLAYER_POINTS 3

static SDL_Color player_alive_color = { 0x0, 0xF0, 0x0, 0xFF };
static SDL_Color player_dead_color = { 0xF0, 0x11, 0x11, 0xFF };
static SDL_Color aabb_color = { 0xF0, 0x00, 0x0, 0xFF };

void player_shoot(Player *p) {
  float x, y, v_x, v_y;
  bullet b;

  v_x = maths_cosf(p->rotation);
  v_y = maths_sinf(p->rotation);

  x = v_x + p->pos.x;
  y = v_y + p->pos.y;

  bullet_init(&b, x, y, v_x, v_y);
  
  darray_insert(p->bullets, &b);
  p->can_shoot = SDL_FALSE;
  p->shoot_cooldown = 0;

  return;
}

void update_bullets(Player *p, const game_frame *delta) {
  SDL_Rect viewport;

  if (darray_size(p->bullets) < 1) {
    return;
  }

  renderer_get_viewport(&viewport);

  for (unsigned int i = 0; i < darray_size(p->bullets);) {
    bullet *b = (bullet*)darray_get(p->bullets, i);
    bullet_update(b, delta);
    keep_in_bounds(&b->sprite, &viewport);

    // clean up "dead" bullets
    if (b->health < 0.f) {
      darray_remove(p->bullets, i);
    } else {
      i++;
    }
  }
}

void draw_bullets(const Player *p) {
  for (unsigned int i = 0; i < darray_size(p->bullets); i++) {
    bullet_draw((bullet*)darray_get(p->bullets, i));
  }
}

void player_move(Player *p) {
  p->pos.x += p->velocity.x;
  p->pos.y += p->velocity.y;
}

vec2f get_normalized_player_direction(Player *p) {
  vec2f normalized;

  // TODO: This may already be normalized...
  normalized.x = maths_cosf(p->rotation);
  normalized.y = maths_sinf(p->rotation);

  normalized = vec2f_normalize(&normalized);

  return normalized;
}

void player_decay_movement(Player *p, float rate, const game_frame *delta) {
  vec2f brake = vec2f_normalize(&p->velocity);

  brake.x *= PLAYER_BRAKE_SPEED * -rate * delta->sec;
  brake.y *= PLAYER_BRAKE_SPEED * -rate * delta->sec;

  if (maths_fabsf(p->velocity.x) - maths_fabsf(brake.x) < 0) {
    p->velocity.x = 0;
  } else {
    p->velocity.x += brake.x;
  }

  if (maths_fabsf(p->velocity.y) - maths_fabsf(brake.y) < 0) {
    p->velocity.y = 0;
  } else {
    p->velocity.y += brake.y;
  }
}

void player_update(Player *p, const game_input *input, const game_frame *delta) {
  vec2f movement;

  if (p->alive == SDL_TRUE) {
    p->shoot_cooldown += delta->mil;
    if (p->shoot_cooldown > PLAYER_SHOOT_COOLDOWN_MS) {
      p->can_shoot = SDL_TRUE;
    }

    if (input->left == SDL_TRUE) {
      p->rotation -= PLAYER_ROT_SPEED * delta->sec;
    }
    
    if (input->right == SDL_TRUE) {
      p->rotation += PLAYER_ROT_SPEED * delta->sec;
    }

    if (p->rotation < 0.f) {
      p->rotation = p->rotation + MATHS_2_PI;
    } else if (p->rotation > MATHS_2_PI) {
      p->rotation = MATHS_2_PI - p->rotation;
    }

    if (input->thrust == SDL_TRUE) {
      movement = get_normalized_player_direction(p);

      movement.x *= PLAYER_THRUST_BASE_SPEED;
      movement.y *= PLAYER_THRUST_BASE_SPEED;

      p->velocity.x = movement.x * delta->sec;
      p->velocity.y = movement.y * delta->sec;
    }

    if (input->brake || p->alive == SDL_FALSE) {
      player_decay_movement(p, 1.f, delta);
    }

    if (input->fire == SDL_TRUE && p->can_shoot) {
      player_shoot(p);
    }
  } else {
    if (p->velocity.x != 0.f || p->velocity.y != 0.f) {
      player_decay_movement(p, 0.25f, delta);
    }
  }

  update_bullets(p, delta);
  player_move(p);
}

void player_draw(const Player *p) {
  SDL_Color player_color;
  shape global;

  if (p->alive == SDL_TRUE) {
    player_color = player_alive_color;
  } else {
    player_color = player_dead_color;
  }

  shape_init(&global, p->sprite.points, p->sprite.num_points);
  shape_rotate(&global, p->rotation);
  shape_translate(&global, p->pos);

  draw_bullets(p);
  shape_draw(&global, &player_color);
}

void player_init(Player *p, float x, float y) {
  p->can_shoot = SDL_TRUE;
  p->shoot_cooldown = 0;

  p->alive = SDL_TRUE;

  p->rotation = MATHS_PI + MATHS_PI_2;
  
  p->velocity.x = 0.f;
  p->velocity.y = 0.f;

  shape_init(&p->sprite, (vec2f[]){
    PLAYER_POINT_0,
    PLAYER_POINT_1,
    PLAYER_POINT_2
  },
  PLAYER_POINTS);
  shape_rotate(&p->sprite, MATHS_PI_2);

  p->pos = (vec2f){ x, y };  

  p->bullets = darray_create(10, sizeof(bullet));

  return;
}

void player_destroy(Player *p) {
  p->velocity = (vec2f){ 0.f, 0.f };
  p->pos = (vec2f){ 0.f, 0.f };
  shape_destroy(&p->sprite);
  darray_destroy(p->bullets);
  p->rotation = 0.f;
  p->can_shoot = 0;
  p->alive = 0;
  p->shoot_cooldown = 0;
}
