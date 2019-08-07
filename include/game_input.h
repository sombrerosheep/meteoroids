#ifndef GAME_INPUT_H
#define GAME_INPUT_H

#include <SDL.h>

typedef struct game_key_bindings {
  SDL_Scancode left;
  SDL_Scancode right;
  SDL_Scancode thrust;
  SDL_Scancode fire;
} game_key_bindings;

typedef struct game_input {
  SDL_bool left;
  SDL_bool right;
  SDL_bool thrust;
  SDL_bool fire;
} game_input;

game_key_bindings* key_bindings_get_default();
game_input game_input_state(game_key_bindings *bindings);
void game_input_set_key_bindings(game_key_bindings *bindings);

#endif