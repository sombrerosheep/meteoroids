#ifndef BULLET_H
#define BULLET_H

#include <rectf.h>
#include <vec2f.h>
#include <game_clock.h>

typedef struct bullet {
  rectf sprite;
  vec2f velocity;
  float health;
} bullet;

void bullet_init(bullet *b, float x, float y, float v_x, float v_y);
void bullet_update(bullet *b, const game_frame *delta);
void bullet_draw(bullet *b);

#endif