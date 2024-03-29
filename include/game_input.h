#ifndef GAME_INPUT_H
#define GAME_INPUT_H

#include <SDL.h>

typedef struct game_key_bindings {
  SDL_Scancode left;
  SDL_Scancode right;
  SDL_Scancode thrust;
  SDL_Scancode brake;
  SDL_Scancode fire;
} game_key_bindings;

typedef struct game_input {
  SDL_bool left;
  SDL_bool right;
  SDL_bool thrust;
  SDL_bool brake;
  SDL_bool fire;
} game_input;

void key_bindings_set_default(game_key_bindings *bindings);
game_input game_input_state(game_key_bindings *bindings);
void game_input_set_key_bindings(game_key_bindings *bindings);

#endif