#include <stdio.h>
#include <game.h>

int main(int argc, char* argv) {
  game_context ctx;
  game_key_bindings bindings;

  key_bindings_set_default(&bindings);

  if (game_init(&ctx, &bindings) != 0) {
    exit(-1);
  }

  game_start(&ctx);

  game_destroy(&ctx);

  return 0;
}