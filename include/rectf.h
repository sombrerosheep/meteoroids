#ifndef RECTF_H
#define RECTF_H

#include <SDL.h>
#include <vec2f.h>

typedef struct rectf {
  float x, y;
  float h, w;
} rectf;

SDL_Rect rectf_to_rect(const rectf *rect);
vec2f rectf_center(const rectf *rect);

#endif