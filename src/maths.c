#include <maths.h>

#include <SDL.h>

float maths_fabsf(float x) {
  return SDL_fabsf(x);
}

float maths_cosf(float radians) {
  return SDL_cosf(radians);
}

float maths_sinf(float radians) {
  return SDL_sinf(radians);
}

float maths_powf(float x, float p) {
  return SDL_powf(x, p);
}

float maths_sqrtf(float x) {
  return SDL_sqrtf(x);
}