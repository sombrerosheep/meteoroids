#ifndef MENUS_H
#define MENUS_H

#include <SDL.h>
#include <text.h>
#include <game_clock.h>

typedef struct main_menu {
  text title;
  text *options;
  int selected;
} main_menu;

typedef struct pause_menu {
  text title;
  text sub_text;
} pause_menu;

void main_menu_init(main_menu *menu, font *title_font, font *sub_font);
void pause_menu_init(pause_menu *menu, font *title_font, font *sub_font);

void main_menu_update(main_menu *menu, const SDL_Event *event, const game_frame *delta);
void pause_menu_update(pause_menu *menu, const SDL_Event *event, const game_frame *delta);

void main_menu_draw(main_menu *menu);
void pause_menu_draw(pause_menu *menu);

void main_menu_destroy(main_menu *menu);
void pause_menu_destroy(pause_menu *menu);

#endif