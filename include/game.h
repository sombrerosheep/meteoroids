#ifndef GAME_H
#define GAME_H

#include <game_input.h>

struct game_context* game_init(game_key_bindings *key_bindings);
void game_start(struct game_context *ctx);
void game_free(struct game_context *ctx);

#endif