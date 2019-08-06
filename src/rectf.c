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