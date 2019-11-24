#ifndef TEXT_H
#define TEXT_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <rectf.h>

typedef struct font {
  TTF_Font    *ttffont;
  int         font_size;
} font;

typedef struct text {
  char          *message;
  SDL_Texture   *texture;
  font          *font;
  rectf         rect;
} text;

void font_init(font *f, const char *path, int ptsize);
void font_destroy(font *f);

void text_init(text *t, font *f, const char *message);
void text_destroy(text *t);

void text_center_horizontal(text *t, float width);
void text_center_vertical(text *t, float height);
void text_center(text *t, float height, float width);

#endif