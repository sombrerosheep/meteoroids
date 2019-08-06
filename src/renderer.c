#include <renderer.h>
#include <rectf.h>

static SDL_Renderer *renderer = NULL;

#define RENDERER_DEFINED() if (renderer == NULL) { return; }

void set_renderer(SDL_Renderer *sdl_renderer) {
  if (sdl_renderer != NULL) {
    renderer = sdl_renderer;
  }

  return;
}

void render_fill_rectf(const rectf *rect, SDL_Color color) {
  SDL_Rect rec;

  RENDERER_DEFINED();

  rec = rectf_to_rect(rect);
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  SDL_RenderFillRect(renderer, &rec);

  return;
}

void render_draw_rectf(const rectf *rect, SDL_Color color) {
  SDL_Rect rec;

  RENDERER_DEFINED();

  rec = rectf_to_rect(rect);
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawRect(renderer, &rec);

  return;
}

void render_fill_rectfs(const rectf *rect, int num_rects, SDL_Color color) {
  SDL_Rect *recs;

  RENDERER_DEFINED();
  
  recs = (SDL_Rect*)malloc(sizeof(SDL_Rect) * num_rects);

  for (int i = 0; i < num_rects; i++) {
    recs[i] = rectf_to_rect(&rect[i]);
  }

  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  SDL_RenderFillRects(renderer, recs, num_rects);

  SDL_free(recs);
  return;
}

void render_draw_rectfs(const rectf *rect, int num_rects, SDL_Color color) {
  SDL_Rect *recs;

  RENDERER_DEFINED();
  
  recs = (SDL_Rect*)malloc(sizeof(SDL_Rect) * num_rects);

  for (int i = 0; i < num_rects; i++) {
    recs[i] = rectf_to_rect(&rect[i]);
  }

  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawRects(renderer, recs, num_rects);

  SDL_free(recs);
  return;
}
