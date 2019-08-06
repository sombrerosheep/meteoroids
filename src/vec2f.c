#include <vec2f.h>

float vec2f_magnitude(const vec2f *rect) {
  float x_sq, y_sq, mag;

  x_sq = SDL_powf(rect->x, 2);
  y_sq = SDL_powf(rect->y, 2);
  mag = SDL_sqrtf(x_sq + y_sq);

  return mag;
}

vec2f vec2f_normalize(const vec2f *rect) {
  vec2f unit;
  float mag;

  mag = vec2f_magnitude(rect);
  unit.x = rect->x / mag;
  unit.y = rect->y / mag;

  return unit;
}