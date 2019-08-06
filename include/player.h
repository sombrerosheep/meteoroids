#ifndef PLAYER_H
#define PLAYER_H

#include <rectf.h>
#include <vec2f.h>
#include <gameinput.h>

typedef struct Player {
  vec2f position;
  vec2f velocity;
  rectf sprite;
  float rotation;
} Player;

void player_update(const Player *p, const game_input *input);
void player_draw(const Player *p);

#endif