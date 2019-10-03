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

SDL_bool keep_in_bounds(rectf *entity, const SDL_Rect *viewport) {
  SDL_bool adjusted = SDL_FALSE;

  if (entity->x + entity->w < 0) {
    entity->x += viewport->w;
    adjusted = SDL_TRUE;
  }
  if (entity->x > viewport->w) {
    entity->x -= viewport->w;
    adjusted = SDL_TRUE;
  }
  if (entity->y + entity->h < 0) {
    entity->y += viewport->h;
    adjusted = SDL_TRUE;
  }
  if (entity->y > viewport->h) {
    entity->y -= viewport->h;
    adjusted = SDL_TRUE;
  }

  return adjusted;
}

SDL_bool rectf_outersects(rectf *rec, const SDL_Rect *viewport, vec2f *offset) {
  SDL_bool adjusted = SDL_FALSE;
  rectf entity = *rec;

  if (entity.x + entity.w < 0) {
    entity.x += viewport->w;
    adjusted = SDL_TRUE;
  }
  if (entity.x > viewport->w) {
    entity.x -= viewport->w;
    adjusted = SDL_TRUE;
  }
  if (entity.y + entity.h < 0) {
    entity.y += viewport->h;
    adjusted = SDL_TRUE;
  }
  if (entity.y > viewport->h) {
    entity.y -= viewport->h;
    adjusted = SDL_TRUE;
  }

  offset->x = entity.x - rec->x;
  offset->y = entity.y - rec->y;

  return adjusted;
}

SDL_bool rectf_intersects_rectf(const rectf *a, const rectf *b) {
  float a_min, a_max;
  float b_min, b_max;

  a_min = a->x;
  a_max = a->x + a->w;
  b_min = b->x;
  b_max = b->x + b->w;

  if (b_min > a_min) {
    a_min = b_min;
  }
  if (b_max < a_max) {
    a_max = b_max;
  }
  if (a_max <= a_min) {
    return SDL_FALSE;
  }

  a_min = a->y;
  a_max = a->y + a->h;
  b_min = b->y;
  b_max = b->y + b->h;

  if (b_min > a_min) {
    a_min = b_min;
  }
  if (b_max < a_max) {
    a_max = b_max;
  }
  if (a_max <= a_min) {
    return SDL_FALSE;
  }

  return SDL_TRUE;
}

int rectf_intersects_circle(const rectf *a, float x, float y, float radius) {
  vec2f ul, ur, ll, lr;
  float mag, max;
  int corner;

  max = 0.f;
  corner = -1;

  ul = (vec2f){a->x - x, a->y - y};
  ur = (vec2f){a->x + a->w - x, a->y - y};
  ll = (vec2f){a->x - x, a->y + a->h - y};
  lr = (vec2f){a->x + a->w - x, a->y + a->h - y};

  mag = vec2f_magnitude(&ul);
  if (mag > radius && mag < max) {
    max = mag;
    corner = 0;
  }
  mag = vec2f_magnitude(&ur);
  if (mag > radius && mag < max) {
    max = mag;
    corner = 1;
  }
  mag = vec2f_magnitude(&ll);
  if (mag > radius && mag < max) {
    max = mag;
    corner = 2;
  }
  mag = vec2f_magnitude(&lr);
  if (mag > radius && mag < max) {
    max = mag;
    corner = 3;
  }

  return corner;
}