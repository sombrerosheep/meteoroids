#include <text.h>
#include <string.h>

void font_init(font *f, const char *path, int ptsize) {
  f->ttffont = TTF_OpenFont(path, ptsize);
  f->font_size = ptsize;

  return;
}

void font_destroy(font *f) {
  TTF_CloseFont(f->ttffont);
  
  f->ttffont = NULL;
  f->font_size = 0;
}

void text_init(text *t, font *f, const char *message) {
  int w, h;

  t->font = f;

  size_t message_length = SDL_strlen(message) + 1;
  t->message = SDL_malloc(message_length);
  SDL_memcpy(t->message, message, message_length);

  t->texture = NULL;

  TTF_SizeText(t->font->ttffont, t->message, &w, &h);
  t->rect.h = h;
  t->rect.w = w;
  t->rect.x = 0.f;
  t->rect.y = 0.f;
}

void text_destroy(text *t) {
  SDL_free(t->message);
  if (t->texture != NULL) {
    SDL_DestroyTexture(t->texture);
  }

  t->font = NULL;
  t->rect = (rectf){ 0.f, 0.f, 0.f, 0.f };
}

void text_center_horizontal(text *t, float width) {
  t->rect.x = width / 2 - t->rect.w / 2;
}

void text_center_vertical(text *t, float height) {
t->rect.y = height / 2 - t->rect.h / 2;
}

void text_center(text *t, float height, float width) {
  text_center_horizontal(t, width);
  text_center_vertical(t, height);
}

