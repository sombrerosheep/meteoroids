#include <renderer.h>
#include <rectf.h>

static SDL_Renderer *renderer = NULL;

#define RENDERER_DEFINED() if (renderer == NULL) { return; }

void renderer_set(SDL_Renderer *sdl_renderer) {
  if (sdl_renderer != NULL) {
    renderer = sdl_renderer;
  }

  return;
}

void renderer_get_viewport(SDL_Rect *viewport) {
  SDL_RenderGetViewport(renderer, viewport);
}

void render_fill_rectf(const rectf *rect, const SDL_Color *color) {
  SDL_Rect rec;

  RENDERER_DEFINED();

  SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
  SDL_RenderFillRectF(renderer, rect);

  return;
}

void render_draw_rectf(const rectf *rect, const SDL_Color *color) {
  SDL_Rect rec;

  RENDERER_DEFINED();

  SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
  SDL_RenderDrawRectF(renderer, rect);

  return;
}

void render_draw_line(const vec2f *start, const vec2f *end, const SDL_Color *color) {
  RENDERER_DEFINED();

  SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
  SDL_RenderDrawLineF(renderer, start->x, start->y, end->x, end->y);

  return;
}

void render_fill_rectfs(const rectf *rect, int num_rects, const SDL_Color *color) {
  SDL_Rect *recs;

  RENDERER_DEFINED();
  
  SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
  SDL_RenderFillRectsF(renderer, rect, num_rects);

  return;
}

void render_draw_rectfs(const rectf *rect, int num_rects, const SDL_Color *color) {
  SDL_Rect *recs;

  RENDERER_DEFINED();

  SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
  SDL_RenderDrawRectsF(renderer, rect, num_rects);

  return;
}
