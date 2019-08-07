#include <player.h>
#include <renderer.h>

#define PLAYER_SPRITE_SIZE 15.f

#define PLAYER_ROT_SPEED 1.f
#define PLAYER_THRUST_BASE_SPEED 0.25f
#define PLAYER_THRUST_MAX_SPEED 2.f

#define PLAYER_CROSSHAIR_OFFSET 10.f
#define BULLET_SPEED 10.f

static SDL_Color player_color = { 0x0, 0xF0, 0x0, 0xFF };

void player_rotate(Player *p) {
  float cross_x, cross_y;
  vec2f player_center;

  cross_x = SDL_cosf(p->rotation);
  cross_y = SDL_sinf(p->rotation);

  cross_x *= PLAYER_CROSSHAIR_OFFSET;
  cross_y *= PLAYER_CROSSHAIR_OFFSET;

  player_center = rectf_center(&p->sprite);

  cross_x += player_center.x;
  cross_y += player_center.y;

  p->crosshair.x = cross_x + p->crosshair.w / 2;
  p->crosshair.y = cross_y + p->crosshair.h / 2;

  printf("rot: %f\n", p->rotation);
}

void player_shoot(Player *p) {
  int cross_x, cross_y;
  vec2f bullet_vec;

  cross_x = SDL_cosf(p->rotation);
  cross_y = SDL_sinf(p->rotation);

}

void player_move(Player *p) {
  p->sprite.x += p->velocity.x;
  p->sprite.y += p->velocity.y;

  p->crosshair.x += p->velocity.x;
  p->crosshair.y += p->velocity.y;
}

void player_update(Player *p, const game_input *input) {
  vec2f movement;

  if (input->left == SDL_TRUE) {
    p->rotation -= PLAYER_ROT_SPEED;
  }
  
  if (input->right == SDL_TRUE) {
    p->rotation += PLAYER_ROT_SPEED;
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

    p->velocity.x += movement.x;
    p->velocity.y += movement.y;
  }

  if (input->fire == SDL_TRUE) {
    player_shoot(p);
  }

  vec2f_clamp(&p->velocity, PLAYER_THRUST_MAX_SPEED);
  player_rotate(p);
  player_move(p);
}

void player_draw(const Player *p) {
  vec2f player_cen, cross_cen;

  player_cen = rectf_center(&p->sprite);
  cross_cen = rectf_center(&p->crosshair);

  render_draw_line(&player_cen, &cross_cen, &player_color);
  render_draw_rectf(&p->sprite, &player_color);
  render_draw_rectf(&p->crosshair, &player_color);
}

void player_init(Player *p, float x, float y) {
  p->rotation = 33.f;
  
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

  player_rotate(p);
}

void player_free(Player *p) {
  SDL_free(p);

  SDL_memset(p, 0, sizeof(Player));
}