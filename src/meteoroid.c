#include <meteoroid.h>
#include <renderer.h>

#define METEOROID_SPEED 50.f

#define FIRST_GEN_SIZE 60.f
#define SECOND_GEN_SIZE 30.f
#define THIRD_GEN_SIZE 15.f
#define FIRST_GEN_HEALTH 30
#define SECOND_GEN_HEALTH 20
#define THIRD_GEN_HEALTH 10

const SDL_Color meteoroid_color = { 0x00, 0x00, 0xFF, 0xFF };

void meteoroid_init(meteoroid *m, enum GENERATION gen) {
  m->gen = gen;

  switch (gen) {
    case THIRD:
      m->health = THIRD_GEN_HEALTH;
      m->sprite = (rectf){ 0.f, 0.f, THIRD_GEN_SIZE, THIRD_GEN_SIZE };
      break;
    case SECOND:
      m->health = SECOND_GEN_HEALTH;
      m->sprite = (rectf){ 0.f, 0.f, SECOND_GEN_SIZE, SECOND_GEN_SIZE };
      break;
    case FIRST:
    default:
      m->health = FIRST_GEN_HEALTH;
      m->sprite = (rectf){ 0.f, 0.f, FIRST_GEN_SIZE, FIRST_GEN_SIZE };
      break;
  }
}

void meteoroid_update(meteoroid *m, game_frame *delta) {
  m->sprite.x += m->velocity.x * delta->sec * METEOROID_SPEED;
  m->sprite.y += m->velocity.y * delta->sec * METEOROID_SPEED;
}

void meteoroid_draw(const meteoroid *m) {
  render_draw_rectf(&m->sprite, &meteoroid_color);
}
