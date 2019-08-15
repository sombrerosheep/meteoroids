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

meteoroid* meteoroid_make_and_init(enum GENERATION gen) {
  meteoroid *m;

  m = (meteoroid*)SDL_malloc(sizeof(meteoroid));

  meteoroid_init(m, gen);
  return m;
}

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

void meteoroid_seed_next_generation(dllist *l, enum GENERATION prev_gen, float x, float y) {
  meteoroid *m1, *m2, *m3, *m4;
  enum GENERATION next_gen;

  switch (prev_gen) {
    case FIRST:
      next_gen = SECOND;
      break;
    case SECOND:
      next_gen = THIRD;
      break;
    case THIRD:
    default:
      return;
  }

  // UL
  m1 = meteoroid_make_and_init(next_gen);
  m1->sprite.x = x;
  m1->sprite.y = y;
  m1->velocity.x = random_getf_between(-METEOROID_VELOCITY_RANGE, 0);
  m1->velocity.y = random_getf_between(-METEOROID_VELOCITY_RANGE, 0);
  
  // UR
  m2 = meteoroid_make_and_init(next_gen);
  m2->sprite.x = x + m2->sprite.w;
  m2->sprite.y = y;
  m2->velocity.x = random_getf_between(0, METEOROID_VELOCITY_RANGE);
  m2->velocity.y = random_getf_between(-METEOROID_VELOCITY_RANGE, 0);
  
  // LL
  m3 = meteoroid_make_and_init(next_gen);
  m3->sprite.x = x;
  m3->sprite.y = y + m3->sprite.h;
  m3->velocity.x = random_getf_between(-METEOROID_VELOCITY_RANGE, 0);
  m3->velocity.y = random_getf_between(0, METEOROID_VELOCITY_RANGE);
  
  // LR
  m4 = meteoroid_make_and_init(next_gen);
  m4->sprite.x = x + m4->sprite.w;
  m4->sprite.y = y + m4->sprite.h;
  m4->velocity.x = random_getf_between(0, METEOROID_VELOCITY_RANGE);
  m4->velocity.y = random_getf_between(0, METEOROID_VELOCITY_RANGE);

  dllist_ins(l, m1);
  dllist_ins(l, m2);
  dllist_ins(l, m3);
  dllist_ins(l, m4);
}