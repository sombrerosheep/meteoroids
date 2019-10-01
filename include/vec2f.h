#ifndef VEC2F_H
#define VEC2F_H

#include <SDL.h>

typedef SDL_FPoint vec2f;

float vec2f_magnitude(const vec2f *vec);
vec2f vec2f_normalize(const vec2f *vec);
vec2f vec2f_normalizef(float x, float y);
void vec2f_clamp(vec2f *vec, float val);

#endif