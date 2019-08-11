#include <bullet.h>

#include <renderer.h>

#define BULLET_SIZE 3.f
#define BULLET_VELOCITY_SPEED 2.f
#define BULLET_INIT_HEALTH 20

const SDL_Color bullet_color = { 0xFF, 0x0, 0x0, 0xFF };

void bullet_init(bullet *b, float x, float y, float v_x, float v_y) {
  vec2f normalized_velocity;

  normalized_velocity = vec2f_normalizef(v_x, v_y);

  b->sprite.x = x;
  b->sprite.y = y;
  b->sprite.w = BULLET_SIZE;
  b->sprite.h = BULLET_SIZE;

  b->velocity.x = normalized_velocity.x * BULLET_VELOCITY_SPEED;
  b->velocity.y = normalized_velocity.y * BULLET_VELOCITY_SPEED;

  b->health = BULLET_INIT_HEALTH;
}

void bullet_update(bullet *b) {
  b->sprite.x += b->velocity.x;
  b->sprite.y += b->velocity.y;
}

void bullet_draw(bullet *b) {
  render_fill_rectf(&b->sprite, &bullet_color);
}