#ifndef GAME_H
#define GAME_H

#include <game_input.h>
#include <player.h>

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

int game_init(game_context *ctx, game_key_bindings *key_bindings);
void game_start(struct game_context *ctx);
void game_destroy(struct game_context *ctx);

#endif