#include <game_input.h>

void game_input_clear(game_input *input) {
  input->left = SDL_FALSE;
  input->right = SDL_FALSE;
  input->thrust = SDL_FALSE;
  input->brake = SDL_FALSE;
  input->fire = SDL_FALSE;
}

game_input game_input_state(game_key_bindings *bindings) {
  const Uint8 *keys;
  game_input input;

  game_input_clear(&input);
  keys = SDL_GetKeyboardState(NULL);

  if (keys[bindings->left]) {
    input.left = SDL_TRUE;
  }

  if (keys[bindings->right]) {
    input.right = SDL_TRUE;
  }

  if (keys[bindings->thrust]) {
    input.thrust = SDL_TRUE;
  }

  if (keys[bindings->brake]) {
    input.brake = SDL_TRUE;
  }

  if (keys[bindings->fire]) {
    input.fire = SDL_TRUE;
  }

  return input;
}

game_key_bindings* key_bindings_get_default() {
  game_key_bindings *bindings;

  bindings = (game_key_bindings*)SDL_malloc(sizeof(game_key_bindings));

  bindings->left = SDL_SCANCODE_A;
  bindings->right = SDL_SCANCODE_D;
  bindings->thrust = SDL_SCANCODE_W;
  bindings->brake = SDL_SCANCODE_S;
  bindings->fire = SDL_SCANCODE_SPACE;

  return bindings;
}