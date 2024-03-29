#ifndef PLAYER_H
#define PLAYER_H

#include <rectf.h>
#include <vec2f.h>
#include <game_input.h>
#include <darray.h>
#include <game_clock.h>
#include <shape.h>

typedef struct Player {
  vec2f velocity;
  vec2f pos;
  shape sprite;
  darray *bullets;
  float rotation;
  SDL_bool can_shoot;
  SDL_bool alive;
  unsigned int shoot_cooldown;
} Player;

void player_init(Player *p, float x, float y);
void player_destroy(Player *p);

void player_update(Player *p, const game_input *input, const game_frame *delta);
void player_draw(const Player *p);

#endif
