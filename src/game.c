#include <game.h>
#include <player.h>

#include <SDL.h>
#include <player.h>
#include <renderer.h>

#define SDL_PRINT_ERROR printf("Error from SDL %s\n", SDL_GetError());
#define SDL_REQUIRE_SUCCESS(x)  if ((x) != 0) SDL_PRINT_ERROR
#define SDL_REQUIRE_NOT_NULL(x) if ((x) == NULL) SDL_PRINT_ERROR

typedef struct game_state {
  Player *player;
  game_key_bindings *bindings;
} game_state;

typedef struct game_context {
  SDL_Window *window;
  SDL_Renderer *renderer;
  game_state *state;
} game_context;

void game_state_free(game_state *state) {
  player_free(state->player);

  SDL_free(state->bindings);
  SDL_free(state);

  SDL_memset(state, 0, sizeof(game_state));
}

void game_context_free(game_context *ctx) {
  game_state_free(ctx->state);

  SDL_DestroyRenderer(ctx->renderer);
  SDL_DestroyWindow(ctx->window);
}

void game_update(game_state *state) {
  game_input input;

  input = game_input_state(state->bindings);

  player_update(state->player, &input);
}

void game_draw(game_context *ctx) {
  SDL_SetRenderDrawColor(ctx->renderer, 0x0, 0x0, 0x0, 0xFF);
  SDL_RenderClear(ctx->renderer);

  player_draw(ctx->state->player);

  SDL_RenderPresent(ctx->renderer);
}

game_context* game_init(game_key_bindings *key_bindings) {
  SDL_Window *window;
  SDL_Renderer *renderer;
  game_context *ctx;
  game_state *state;
  Player *player;

  ctx = (game_context*)SDL_malloc(sizeof(game_context));
  state = (game_state*)SDL_malloc(sizeof(game_state));
  player = (Player*)SDL_malloc(sizeof(Player));

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

    game_update(ctx->state);
    game_draw(ctx);
  }
}

void game_free(game_context *ctx) {
  game_context_free(ctx);
}