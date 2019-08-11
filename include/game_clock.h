#ifndef CLOCK_H
#define CLOCK_H

typedef struct game_frame {
  int mil;
  float sec;
} game_frame;

typedef struct game_clock {
  unsigned int start;
} game_clock;

void game_clock_init(game_clock *c);
game_frame game_clock_get_ticks(game_clock *c);
game_frame game_clock_reset(game_clock *c);

#endif