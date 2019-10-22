#include <stdio.h>
#include <game.h>

int main(int argc, char* argv) {
  struct game_context *ctx;

  ctx = game_init(NULL);

  if (ctx == NULL) {
    exit(-1);
  }

  game_start(ctx);

  game_destroy(ctx);

  return 0;
}