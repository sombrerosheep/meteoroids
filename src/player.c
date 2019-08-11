#include <player.h>
#include <renderer.h>
#include <bullet.h>

#define PLAYER_SPRITE_SIZE 15.f

#define PLAYER_ROT_SPEED 100.f
#define PLAYER_THRUST_BASE_SPEED 15.f
#define PLAYER_THRUST_MAX_SPEED 2.f

#define PLAYER_CROSSHAIR_OFFSET 15.f

#define PLAYER_SHOOT_COOLDOWN_MS 900

#define DEG_TO_RAD 0.01745329251994329576924f

static SDL_Color player_color = { 0x0, 0xF0, 0x0, 0xFF };

void destroy_bullet(void *data) {
  bullet *b;

  b = (bullet*)data;

  SDL_free(b);
}

void player_rotate(Player *p) {
  float cross_x, cross_y;
  vec2f player_center;

  cross_x = SDL_cosf(p->rotation * DEG_TO_RAD);
  cross_y = SDL_sinf(p->rotation * DEG_TO_RAD);

  cross_x *= PLAYER_CROSSHAIR_OFFSET;
  cross_y *= PLAYER_CROSSHAIR_OFFSET;

  player_center = rectf_center(&p->sprite);

  cross_x += player_center.x;
  cross_y += player_center.y;

  p->crosshair.x = cross_x - p->crosshair.w / 2;
  p->crosshair.y = cross_y - p->crosshair.h / 2;
}

void player_shoot(Player *p) {
  float x, y, v_x, v_y;
  vec2f bullet_vec, player_center;
  bullet *b;

  b = SDL_malloc(sizeof(bullet));

  v_x = SDL_cosf(p->rotation * DEG_TO_RAD);
  v_y = SDL_sinf(p->rotation * DEG_TO_RAD);
  player_center = rectf_center(&p->sprite);

  x = v_x + player_center.x;
  y = v_y + player_center.y;

  bullet_init(b, x, y, v_x, v_y);
  
  dllist_ins(&p->bullets, b);
  p->can_shoot = SDL_FALSE;
  p->shoot_cooldown = 0;

  return;
}

void update_bullets(Player *p, const SDL_Rect *viewport, const game_frame *delta) {
  dllist_element *e;
  bullet *b;

  for (e = p->bullets.head; e != NULL;) {
    b = (bullet*)e->data;
    bullet_update(b, delta);
    keep_in_bounds(&b->sprite, viewport);

    // clean up "dead" bullets
    if (((bullet*)e->data)->health < 0.f) {
      dllist_element *prev = e->prev; // needed to correct iterator
      int res = dllist_rem(&p->bullets, (void**)&b);
      destroy_bullet(b);
      e = prev; // correct iterator
    } else {
      e = e->next;
    }
  }
}

void draw_bullets(const Player *p) {
  dllist_element *e;

  for (e = p->bullets.head; e != NULL; e = e->next) {
    bullet_draw((bullet*)e->data);
  }
}

void player_move(Player *p) {
  p->sprite.x += p->velocity.x;
  p->sprite.y += p->velocity.y;

  p->crosshair.x += p->velocity.x;
  p->crosshair.y += p->velocity.y;
}

void player_update(Player *p, const game_input *input, const SDL_Rect *viewport, const game_frame *delta) {
  vec2f movement;

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
    p->rotation = p->rotation + 360.f;
  } else if (p->rotation > 360.f) {
    p->rotation = 360 - p->rotation;
  }

  if (input->thrust == SDL_TRUE) {
    movement.x = p->crosshair.x - p->sprite.x;
    movement.y = p->crosshair.y - p->sprite.y;

    movement = vec2f_normalize(&movement);

    movement.x *= PLAYER_THRUST_BASE_SPEED;
    movement.y *= PLAYER_THRUST_BASE_SPEED;

    p->velocity.x += movement.x * delta->sec;
    p->velocity.y += movement.y * delta->sec;
  }

  if (input->fire == SDL_TRUE && p->can_shoot) {
    player_shoot(p);
  }

  update_bullets(p, viewport, delta);
  vec2f_clamp(&p->velocity, PLAYER_THRUST_MAX_SPEED);
  player_rotate(p);
  player_move(p);
}

void player_draw(const Player *p) {
  vec2f player_cen, cross_cen;

  player_cen = rectf_center(&p->sprite);
  cross_cen = rectf_center(&p->crosshair);

  draw_bullets(p);
  render_draw_line(&player_cen, &cross_cen, &player_color);
  render_fill_rectf(&p->sprite, &player_color);
  render_fill_rectf(&p->crosshair, &player_color);
}

void player_init(Player *p, float x, float y) {
  p->can_shoot = SDL_TRUE;
  p->shoot_cooldown = 0;

  p->rotation = 270.f;
  
  p->velocity.x = 0.f;
  p->velocity.y = 0.f;
  
  p->sprite.x = x;
  p->sprite.y = y;
  p->sprite.w = PLAYER_SPRITE_SIZE;
  p->sprite.h = PLAYER_SPRITE_SIZE;

  p->crosshair.x = 0.f;
  p->crosshair.y = 0.f;
  p->crosshair.w = 5.f;
  p->crosshair.h = 5.f;

  dllist_init(&p->bullets, destroy_bullet);

  player_rotate(p);
}

void player_free(Player *p) {
  dllist_destroy(&p->bullets);

  SDL_free(p);

  SDL_memset(p, 0, sizeof(Player));
}