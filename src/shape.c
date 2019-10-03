#include <shape.h>

#include <renderer.h>

void shape_init(shape *s, vec2f *p, int count) {
  s->num_points = count;
  s->points = SDL_malloc(sizeof(vec2f) * count);

  for (int i = 0; i < count; i++) {
    s->points[i] = p[i];
  }

  return;
}

rectf shape_aabb(const shape *s) {
  rectf aabb;
  float min_x, max_x;
  float min_y, max_y;

  min_x = s->points[0].x;
  max_x = s->points[0].x;
  min_y = s->points[0].y;
  max_y = s->points[0].y;

  for (int i = 1; i < s->num_points; i++) {
    vec2f v = s->points[i];

    if (v.x < min_x) {
      min_x = v.x;
    } else if (v.x > max_x) {
      max_x = v.x;
    }

    if (v.y < min_y) {
      min_y = v.y;
    } else if (v.y > max_y) {
      max_y = v.y;
    }
  }

  aabb.x = min_x;
  aabb.y = min_y;
  aabb.w = SDL_fabsf(max_x - min_x);
  aabb.h = SDL_fabsf(max_y - min_y);

  return aabb;
}

void shape_translate(shape *s,vec2f offset) {
  for (int i = 0; i < s->num_points; i++) {
    vec2f p = s->points[i];

    s->points[i] = (vec2f){
      p.x + offset.x,
      p.y + offset.y
    };
  }

  return;
}

void shape_rotate(shape *s, float angle) {
  float angle_sin = SDL_sinf(angle);
  float angle_cos = SDL_cosf(angle);

  for (int i = 0; i < s->num_points; i++) {
    vec2f p = s->points[i];
    float rot_x = angle_cos * p.x + -angle_sin * p.y;
    float rot_y = angle_sin * p.x + angle_cos * p.y;

    s->points[i] = (vec2f){ rot_x, rot_y };
  }

  return;
}

void shape_draw(const shape *s, const SDL_Color *c) {
  render_draw_lines(s->points, s->num_points, c);
}

void shape_free(shape *s) {
  SDL_free(s->points);
  s->points = NULL;
  s->num_points = 0;

  return;
}