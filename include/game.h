#ifndef GAME_H
#define GAME_H

#include <game_input.h>
#include <player.h>
#include <resources.h>
#include <text.h>

typedef enum game_mode {
  GAME_MODE_MENU,
  GAME_MODE_PLAY,
  GAME_MODE_PAUSE
} game_mode;

typedef struct game_state {
  game_key_bindings     *bindings;
  Player                *player;
  dllist                *meteoroids;
  game_mode             mode;
  text                  *title_text;
  text                  *title_desc_text;
  text                  *pause_text;
  text                  *pause_desc_text;
  text                  *dead_text;
  text                  *dead_desc_text;
} game_state;

typedef struct game_context {
  SDL_Window        *window;
  SDL_Renderer      *renderer;
  resources         *resources;
  game_state        *state;
} game_context;

int game_init(game_context *ctx, game_key_bindings *key_bindings);
void game_start(struct game_context *ctx);
void game_destroy(struct game_context *ctx);

#endif