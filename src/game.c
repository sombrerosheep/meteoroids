#include <game.h>
#include <player.h>

#include <SDL.h>
#include <player.h>
#include <renderer.h>
#include <dllist.h>
#include <meteoroid.h>

#define SDL_PRINT_ERROR printf("Error from SDL %s\n", SDL_GetError());
#define SDL_REQUIRE_SUCCESS(x)  if ((x) != 0) SDL_PRINT_ERROR
#define SDL_REQUIRE_NOT_NULL(x) if ((x) == NULL) SDL_PRINT_ERROR

#define NUM_STARTING_METEOROIDS 8
#define METEOROID_VELOCITY_RANGE 1.2f

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

void keep_in_bounds(rectf *entity, const SDL_Rect *viewport) {
  if (entity->x + entity->w < 0) {
    entity->x += viewport->w;
  }
  if (entity->x > viewport->w) {
    entity->x -= viewport->w;
  }
  if (entity->y + entity->h < 0) {
    entity->y += viewport->h;
  }
  if (entity->y > viewport->h) {
    entity->y -= viewport->h;
  }
}

void update_entity_positions(game_context *ctx) {
  SDL_Rect viewport;
  dllist_element *e;

  SDL_RenderGetViewport(ctx->renderer, &viewport);
  
  keep_in_bounds(&ctx->state->player->sprite, &viewport);
  
  for (e = ctx->state->meteoroids->head; e != NULL; e = e->next) {
    keep_in_bounds(&((meteoroid*)e->data)->sprite, &viewport);
  }
}

void destroy_meteoroid(void *data) {
  meteoroid *m;

  m = (meteoroid*)data;

  SDL_free(m);
}

void update_meteoroids(dllist *l) {
  dllist_element *e;

  for (e = l->head; e != NULL; e = e->next) {
    meteoroid_update((meteoroid*)e->data);
  }
}

void draw_meteoroids(dllist *l) {
  dllist_element *e;

  for (e = l->head; e != NULL; e = e->next) {
    meteoroid_draw((meteoroid*)e->data);
  }
}

void build_game_world(game_state *state, SDL_Renderer *renderer) {
  float x, y, v_x, v_y;
  SDL_Rect viewport;
  meteoroid *m;

  SDL_RenderGetViewport(renderer, &viewport);

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

    dllist_ins(state->meteoroids, m);
  }
}

void game_update(game_context *ctx) {
  game_input input;

  input = game_input_state(ctx->state->bindings);

  player_update(ctx->state->player, &input);
  update_meteoroids(ctx->state->meteoroids);

  update_entity_positions(ctx);
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

  ctx = (game_context*)SDL_malloc(sizeof(game_context));
  state = (game_state*)SDL_malloc(sizeof(game_state));
  player = (Player*)SDL_malloc(sizeof(Player));
  meteoroids = (dllist*)SDL_malloc(sizeof(dllist));

  SDL_REQUIRE_SUCCESS(SDL_Init(SDL_INIT_EVERYTHING));
  SDL_REQUIRE_NOT_NULL(window = SDL_CreateWindow("Meteoroids", 200, 125, 800, 600, SDL_WINDOW_OPENGL));
  SDL_REQUIRE_NOT_NULL(renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));

  player_init(player, 100.f, 100.f);
  state->player = player;

  if (key_bindings != NULL) {
    state->bindings = key_bindings;
  } else {
    state->bindings = key_bindings_get_default();
  }

  dllist_init(meteoroids, destroy_meteoroid);
  state->meteoroids = meteoroids;
  build_game_world(state, renderer);

  ctx->window = window;
  ctx->renderer = renderer;
  ctx->state = state;

  renderer_set(renderer);

  return ctx;
}

void game_start(game_context *ctx) {
  SDL_Event event;
  SDL_bool running;

  running = SDL_TRUE;

  while (running) {
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = SDL_FALSE;
      }

      if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
        running = SDL_FALSE;
      }
    }

    game_update(ctx);
    game_draw(ctx);
  }
}

void game_free(game_context *ctx) {
  game_context_free(ctx);
}