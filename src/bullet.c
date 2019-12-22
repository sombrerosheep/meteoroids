#include <bullet.h>

#include <renderer.h>

#define BULLET_SIZE 3.f
#define BULLET_SPEED 125.f
#define BULLET_INIT_HEALTH 20.f
#define BULLET_DEGREDATION_RATE ((BULLET_INIT_HEALTH) / 3.f)

const SDL_Color bullet_color = { 0xFF, 0x0, 0x0, 0xFF };

void bullet_init(bullet *b, float x, float y, float v_x, float v_y) {
  vec2f normalized_velocity;

  normalized_velocity = vec2f_normalizef(v_x, v_y);

  b->sprite.x = x;
  b->sprite.y = y;
  b->sprite.w = BULLET_SIZE;
  b->sprite.h = BULLET_SIZE;

  b->velocity.x = normalized_velocity.x;
  b->velocity.y = normalized_velocity.y;

  b->health = BULLET_INIT_HEALTH;
}

void bullet_update(bullet *b, const game_frame *delta) {
  // The longer the bullet is alive, the less damage it does
  b->health -= delta->sec * BULLET_DEGREDATION_RATE;

  b->sprite.x += b->velocity.x * delta->sec * BULLET_SPEED;
  b->sprite.y += b->velocity.y * delta->sec * BULLET_SPEED;
}

void bullet_draw(bullet *b) {
  render_fill_rectf(&b->sprite, &bullet_color);
}