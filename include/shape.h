#ifndef SHAPE_H
#define SHAPE_H

#include <vec2f.h>
#include <rectf.h>
#include <SDL.h>

typedef struct shape {
  vec2f *points;
  int num_points;
} shape;

void shape_init(shape *s, vec2f *p, int count);
rectf shape_aabb(const shape *s);
void shape_translate(shape *s, vec2f offset);
void shape_rotate(shape *s, float angle);
void shape_draw(const shape *s, const SDL_Color *c);
void shape_destory(shape *s);

#endif