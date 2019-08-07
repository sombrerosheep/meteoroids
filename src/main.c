#include <stdio.h>
#include <game.h>

int main(int argc, char* argv) {
  struct game_context *ctx;

  ctx = game_init(NULL);
  game_start(ctx);

  game_free(ctx);

  return 0;
}