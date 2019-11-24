#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include <rectf.h>
#include <vec2f.h>
#include <text.h>

void renderer_set(SDL_Renderer *renderer);
void renderer_get_viewport(SDL_Rect *viewport);
void render_fill_rectf(const rectf *rect, const SDL_Color *color);
void render_draw_rectf(const rectf *rect, const SDL_Color *color);
void render_draw_line(const vec2f *start, const vec2f *end, const SDL_Color *color);
void render_fill_rectfs(const rectf *rect, int num_rects, const SDL_Color *color);
void render_draw_rectfs(const rectf *rect, int num_rects, const SDL_Color *color);
void render_draw_lines(const vec2f *points, int num_points, const SDL_Color *color);
void render_fill_text(text *t, const SDL_Color *color);

#endif