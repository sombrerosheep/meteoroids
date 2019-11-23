#ifndef METEOROID_H
#define METEOROID_H

#include <rectf.h>
#include <vec2f.h>
#include <random.h>
#include <game_clock.h>
#include <dllist.h>

#define METEOROID_VELOCITY_RANGE 1.2f

enum GENERATION {
  FIRST   = 0,
  SECOND  = 1,
  THIRD   = 3
};

typedef struct meteoroid {
  float             health;
  rectf             sprite;
  vec2f             velocity;
  enum GENERATION   gen;
} meteoroid;

void meteoroid_init(meteoroid *m, enum GENERATION gen);
void meteoroid_update(meteoroid *m, game_frame *delta);
void meteoroid_draw(const meteoroid *m);
void meteoroid_seed_next_generation(dllist *l, enum GENERATION prev_gen, float x, float y);

#endif