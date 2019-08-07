#include <rectf.h>
#include <SDL.h>

SDL_Rect rectf_to_rect(const rectf *rect) {
  SDL_Rect rec;

  rec.x = rect->x;
  rec.y = rect->y;
  rec.w = rect->w;
  rec.h = rect->h;

  return rec;
}

vec2f rectf_center(const rectf *rect) {
  vec2f cen;

  cen.x = rect->x + rect->w / 2.f;
  cen.y = rect->y + rect->h / 2.f;

  return cen;
}