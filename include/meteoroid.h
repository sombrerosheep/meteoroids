#ifndef METEOROID_H
#define METEOROID_H

#include <rectf.h>
#include <vec2f.h>
#include <random.h>
#include <game_clock.h>

enum GENERATION {
  FIRST,
  SECOND,
  THIRD
};

typedef struct meteoroid {
  int         health;
  rectf       sprite;
  vec2f       velocity;
  enum GENERATION  gen;
} meteoroid;

void meteoroid_init(meteoroid *m, enum GENERATION gen);
void meteoroid_update(meteoroid *m, game_frame *delta);
void meteoroid_draw(const meteoroid *m);

#endif