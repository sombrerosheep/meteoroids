#include <menus.h>

#include <renderer.h>

void main_menu_init(main_menu *menu, font *title_font, font *sub_font) {
  text_init(&menu->title, title_font, "meteoroids");
  text_center(&menu->title, 500, 800);
  text_init(&menu->sub_text, sub_font, "press <enter> to play");
  text_center_horizontal(&menu->sub_text, 800);
  menu->sub_text.rect.y = menu->title.rect.y + menu->title.rect.h + 20.f;
}

void pause_menu_init(pause_menu *menu, font *title_font, font *sub_font) {
  text_init(&menu->title, title_font, "paused");
  text_center(&menu->title, 500, 800);
  text_init(&menu->sub_text, sub_font, "press <enter> to resume");
  text_center_horizontal(&menu->sub_text, 800);
  menu->sub_text.rect.y = menu->title.rect.y + menu->title.rect.h + 20.f;
}

void main_menu_update(main_menu *menu, const SDL_Event *event, const game_frame *delta) {
  return;
}

void pause_menu_update(pause_menu *menu, const SDL_Event *event, const game_frame *delta) {
  return;
}

void main_menu_draw(main_menu *menu) {
  SDL_Color tex_col = { 0xA0, 0x20, 0x80, 0x80};

  render_fill_text(&menu->title, &tex_col);
  render_fill_text(&menu->sub_text, &tex_col);
}

void pause_menu_draw(pause_menu *menu) {
  SDL_Color tex_col = { 0xA0, 0x20, 0x80, 0x80};

  render_fill_text(&menu->title, &tex_col);
  render_fill_text(&menu->sub_text, &tex_col);
}

void main_menu_destroy(main_menu *menu) {
  text_destroy(&menu->title);
  text_destroy(&menu->sub_text);
}

void pause_menu_destroy(pause_menu *menu) {
  text_destroy(&menu->title);
  text_destroy(&menu->sub_text);
}