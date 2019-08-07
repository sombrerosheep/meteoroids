#ifndef VEC2F_H
#define VEC2F_H

typedef struct vec2f {
  float x, y;
} vec2f;

float vec2f_magnitude(const vec2f *vec);
vec2f vec2f_normalize(const vec2f *vec);
void vec2f_clamp(vec2f *vec, float val);

#endif