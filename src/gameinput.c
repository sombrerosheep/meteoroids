#include <gameinput.h>

const game_key_bindings default_key_bindings = {
  SDL_SCANCODE_A,
  SDL_SCANCODE_D,
  SDL_SCANCODE_W,
  SDL_SCANCODE_SPACE
};

static game_key_bindings *current_key_bindings = NULL;

void game_input_clear(game_input *input) {
  input->left = SDL_FALSE;
  input->right = SDL_FALSE;
  input->thrust = SDL_FALSE;
  input->fire = SDL_FALSE;
}

game_input game_input_state() {
  Uint8 *keys;
  game_input input;

  if (current_key_bindings == NULL) {
    current_key_bindings = &default_key_bindings;
  }

  game_input_clear(&input);
  keys = SDL_GetKeyboardState(NULL);

  if (keys[current_key_bindings->left]) {
    input.left = SDL_TRUE;
  }

  if (keys[current_key_bindings->right]) {
    input.right = SDL_TRUE;
  }

  if (keys[current_key_bindings->thrust]) {
    input.thrust = SDL_TRUE;
  }

  if (keys[current_key_bindings->fire]) {
    input.fire = SDL_TRUE;
  }
}

void set_key_bindings(game_key_bindings *bindings) {
  if (bindings == NULL) {
    // set default
  }

  // set provided
}