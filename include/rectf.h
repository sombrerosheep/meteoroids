#ifndef RECTF_H
#define RECTF_H

#include <SDL.h>
#include <vec2f.h>

typedef SDL_FRect rectf;

vec2f rectf_center(const rectf *rect);
SDL_bool keep_in_bounds(rectf *entity, const SDL_Rect *viewport);
SDL_bool rectf_intersects_rectf(const rectf *a, const rectf *b);
SDL_bool rectf_outersects(rectf *rec, const SDL_Rect *viewport, vec2f *offset);
int rectf_intersects_circle(const rectf *a, float x, float y, float radius);

#endif