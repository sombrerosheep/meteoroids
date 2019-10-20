#include <game.h>
#include <player.h>

#include <stdio.h>
#include <SDL.h>
#include <player.h>
#include <renderer.h>
#include <dllist.h>
#include <meteoroid.h>
#include <game_clock.h>
#include <bullet.h>
#include <time.h>

#define SDL_PRINT_ERROR printf("Error from SDL %s\n", SDL_GetError());
#define SDL_REQUIRE_SUCCESS(x)  if ((x) != 0) SDL_PRINT_ERROR
#define SDL_REQUIRE_NOT_NULL(x) if ((x) == NULL) SDL_PRINT_ERROR

#define NUM_STARTING_METEOROIDS 8

typedef struct game_state {
  game_key_bindings *bindings;
  Player *player;
  dllist *meteoroids;

} game_state;

typedef struct game_context {
  SDL_Window *window;
  SDL_Renderer *renderer;
  game_state *state;
} game_context;

void game_state_free(game_state *state) {
  player_free(state->player);
  dllist_destroy(state->meteoroids);

  SDL_free(state->meteoroids);
  SDL_free(state->bindings);
  SDL_free(state);

  SDL_memset(state, 0, sizeof(game_state));
}

void game_context_free(game_context *ctx) {
  game_state_free(ctx->state);

  SDL_DestroyRenderer(ctx->renderer);
  SDL_DestroyWindow(ctx->window);
}

void keep_player_in_bounds(game_context *ctx) {
  SDL_Rect viewport;
  vec2f offset;
  rectf aabb = shape_aabb(&ctx->state->player->sprite);
  aabb.x += ctx->state->player->pos.x;
  aabb.y += ctx->state->player->pos.y;

  renderer_get_viewport(&viewport);
  if (rectf_outersects(&aabb, &viewport, &offset) == SDL_TRUE) {
    ctx->state->player->pos.x += offset.x;
    ctx->state->player->pos.y += offset.y;
  }
}

void destroy_meteoroid(void *data) {
  meteoroid *m;

  m = (meteoroid*)data;

  SDL_free(m);
}

void meteoroid_bullets_collisions(game_state *state, meteoroid *m) {
  dllist *bullets = &state->player->bullets;
  dllist_element *e;

  for (e = bullets->head; e != NULL; e = e->next) {
    bullet *b;
    
    b = (bullet*)e->data;
    if (rectf_intersects_rectf(&b->sprite, &m->sprite) == SDL_TRUE) {
      m->health -= b->health;
      b->health = 0;
    }
  }
}

void resolve_meteoroid_collisions(meteoroid *m, dllist *list) {
  for (dllist_element *e = list->head; e != NULL; e = e->next) {
    vec2f v = m->velocity;
    meteoroid *em = e->data;
    rectf intersect = { 0.f, 0.f, 0.f, 0.f };

    if (em == m) {
      continue;
    }

    // back off the intersection amount and then set the velocity
    if (rectf_intersection(&m->sprite, &em->sprite, &intersect) == SDL_TRUE) {
      if (intersect.h > intersect.w) {
        // assume that hit was left or right
        m->velocity.x *= -1.f;
        if (intersect.x > m->sprite.x) {
          // collision was on right
          m->sprite.x -= intersect.w;
        } else {
          // collision was on left
          m->sprite.x += intersect.w;
        }
      } else {
        // assume that hit was top or bottom
        m->velocity.y *= -1.f;
        if (intersect.y > m->sprite.y) {
          // collision was on bottom
          m->sprite.y -= intersect.h;
        } else {
          // collision was on top
          m->sprite.y += intersect.h;
        }
      }

      return;
    }
  }
}

void update_meteoroids(game_state *state, game_frame *delta) {
  dllist_element *e;
  meteoroid *m;
  SDL_Rect viewport;

  renderer_get_viewport(&viewport);

  for (e = state->meteoroids->head; e != NULL;) {
    m = (meteoroid*)e->data;

    meteoroid_bullets_collisions(state, m);
    meteoroid_update(m, delta);
    keep_in_bounds(&m->sprite, &viewport);

    if (m->health < 0) {
      // handle generations
      dllist_element *prev = e->prev;
      dllist_rem(state->meteoroids, (void**)&m);
      meteoroid_seed_next_generation(state->meteoroids, m->gen, m->sprite.x, m->sprite.y);
      destroy_meteoroid(m);
      e = prev;
    } else {
      e = e->next;
    }

    resolve_meteoroid_collisions(m, state->meteoroids);
  }
}

void draw_meteoroids(dllist *l) {
  dllist_element *e;

  for (e = l->head; e != NULL; e = e->next) {
    meteoroid_draw((meteoroid*)e->data);
  }
}

void build_game_world(game_state *state) {
  float x, y, v_x, v_y;
  float safe_zone;
  int intersects;
  SDL_Rect viewport;
  meteoroid *m;

  safe_zone = 100.f;
  renderer_get_viewport(&viewport);

  for (int i = 0; i < NUM_STARTING_METEOROIDS; i++) {
    m = SDL_malloc(sizeof(meteoroid));
    meteoroid_init(m, FIRST);

    x = random_getf_between((float)viewport.x, (float)viewport.w);
    y = random_getf_between((float)viewport.y, (float)viewport.h);
    v_x = random_getf_between(-METEOROID_VELOCITY_RANGE, METEOROID_VELOCITY_RANGE);
    v_y = random_getf_between(-METEOROID_VELOCITY_RANGE, METEOROID_VELOCITY_RANGE);

    m->velocity.x = v_x;
    m->velocity.y = v_y;
    m->sprite.x = x;
    m->sprite.y = y;

    intersects = rectf_intersects_circle(&m->sprite, 400.f, 300.f, safe_zone);
    if (intersects != -1) {
      switch (intersects) {
        case 0:
          m->sprite.x += 100.f;
          m->sprite.y += 100.f;
          break;
        case 1:
          m->sprite.x -= (100.f + 30.f);
          m->sprite.y += (100.f + 30.f);
          break;
        case 2:
          m->sprite.x += (100.f + 30.f);
          m->sprite.y -= (100.f + 30.f);
          break;
        case 3:
          m->sprite.x -= (100.f + 30.f);
          m->sprite.y -= (100.f + 30.f);
          break;
        default:
          break;
      }
    }

    dllist_ins(state->meteoroids, m);
  }
}

void game_update(game_context *ctx, game_frame *delta) {
  game_input input;

  input = game_input_state(ctx->state->bindings);

  player_update(ctx->state->player, &input, delta);
  update_meteoroids(ctx->state, delta);

  keep_player_in_bounds(ctx);
}

void game_draw(game_context *ctx) {
  SDL_SetRenderDrawColor(ctx->renderer, 0x0, 0x0, 0x0, 0xFF);
  SDL_RenderClear(ctx->renderer);

  player_draw(ctx->state->player);
  draw_meteoroids(ctx->state->meteoroids);

  SDL_RenderPresent(ctx->renderer);
}

game_context* game_init(game_key_bindings *key_bindings) {
  SDL_Window *window;
  SDL_Renderer *renderer;
  game_context *ctx;
  game_state *state;
  Player *player;
  dllist *meteoroids;

  if (!SDL_VERSION_ATLEAST(2, 0, 10)) {
    SDL_version ver;
    SDL_GetVersion(&ver);
    printf("Minimum SDL Version not met.\n\tWant 2.10.0+.\n\tHave: %d.%d.%d\n", ver.major, ver.minor, ver.patch);
    return NULL;
  }

  random_init(clock());

  ctx = (game_context*)SDL_malloc(sizeof(game_context));
  state = (game_state*)SDL_malloc(sizeof(game_state));
  player = (Player*)SDL_malloc(sizeof(Player));
  meteoroids = (dllist*)SDL_malloc(sizeof(dllist));

  SDL_REQUIRE_SUCCESS(SDL_Init(SDL_INIT_EVERYTHING));
  SDL_REQUIRE_NOT_NULL(window = SDL_CreateWindow("Meteoroids", 200, 125, 800, 600, SDL_WINDOW_OPENGL));
  SDL_REQUIRE_NOT_NULL(renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));

  ctx->window = window;
  ctx->renderer = renderer;
  ctx->state = state;

  renderer_set(renderer);

  player_init(player, 400.f, 300.f);
  state->player = player;

  if (key_bindings != NULL) {
    state->bindings = key_bindings;
  } else {
    state->bindings = key_bindings_get_default();
  }

  dllist_init(meteoroids, destroy_meteoroid);
  state->meteoroids = meteoroids;
  build_game_world(state);

  return ctx;
}

void game_start(game_context *ctx) {
  SDL_Event event;
  SDL_bool running;
  game_clock clock;
  game_frame delta;

  game_clock_init(&clock);
  running = SDL_TRUE;

  while (running) {
    delta = game_clock_reset(&clock);
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = SDL_FALSE;
      }

      if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
        running = SDL_FALSE;
      }
    }

    game_update(ctx, &delta);
    game_draw(ctx);
  }
}

void game_free(game_context *ctx) {
  game_context_free(ctx);
}