#include <stdio.h>
#include <game.h>

int main(int argc, char* argv) {
  game_context ctx;

  game_init(&ctx, NULL);

  game_start(&ctx);

  game_destroy(&ctx);

  return 0;
}