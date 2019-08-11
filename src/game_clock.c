#include <game_clock.h>
#include <SDL.h>

#define MILS_IN_SECF 1000.f

void game_clock_init(game_clock *c) {
  c->start = SDL_GetTicks();
}

game_frame game_clock_get_ticks(game_clock *c) {
  unsigned int delta;
  game_frame f;

  delta = SDL_GetTicks() - c->start;

  f.mil = delta;
  f.sec = delta / MILS_IN_SECF;

  return f;
}

game_frame game_clock_reset(game_clock *c) {
  unsigned int ticks, delta;
  game_frame f;

  ticks = SDL_GetTicks();
  delta = ticks - c->start;
  c->start = ticks;

  f.mil = delta;
  f.sec = (float)delta / MILS_IN_SECF;

  return f;
}