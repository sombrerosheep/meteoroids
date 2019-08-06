#ifndef GAME_INPUT_H
#define GAME_INPUT_H

#include <SDL.h>

typedef struct game_key_bindings {
  Uint8 left;
  Uint8 right;
  Uint8 thrust;
  Uint8 fire;
} game_key_bindings;

typedef struct game_input {
  SDL_bool left;
  SDL_bool right;
  SDL_bool thrust;
  SDL_bool fire;
} game_input;

void game_input_clear(game_input *input);
game_input game_input_state();
void set_key_bindings(game_key_bindings *bindings);

#endif