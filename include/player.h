#ifndef PLAYER_H
#define PLAYER_H

#include <rectf.h>
#include <vec2f.h>
#include <game_input.h>
#include <dllist.h>
#include <game_clock.h>


typedef struct Player {
  vec2f velocity;
  rectf crosshair;
  rectf sprite;
  dllist bullets;
  float rotation;
} Player;

void player_init(Player *p, float x, float y);
void player_free(Player *p);

void player_update(Player *p, const game_input *input, const game_frame *delta);
void player_draw(const Player *p);

#endif