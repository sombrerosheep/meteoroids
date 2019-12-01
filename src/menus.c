#include <menus.h>

#include <renderer.h>

void main_menu_init(main_menu *menu, font *title_font, font *sub_font) {
  text_init(&menu->title, title_font, "meteoroids");
  text_center(&menu->title, 500, 800);

  menu->selected = 0;
  menu->options = SDL_malloc(sizeof(text) * MMO_TOTAL);

  text_init(&menu->options[MMO_PLAY], sub_font, "play");
  text_center_horizontal(&menu->options[MMO_PLAY], 800);
  menu->options[MMO_PLAY].rect.y = menu->title.rect.y + menu->title.rect.h + 20.f;

  text_init(&menu->options[MMO_QUIT], sub_font, "quit");
  text_center_horizontal(&menu->options[MMO_QUIT], 800);
  menu->options[MMO_QUIT].rect.y = menu->title.rect.y + menu->title.rect.h + 50.f;
}

void pause_menu_init(pause_menu *menu, font *title_font, font *sub_font) {
  text_init(&menu->title, title_font, "paused");
  text_center(&menu->title, 500, 800);
  
  menu->selected = 0;
  menu->options = SDL_malloc(sizeof(text) * PMO_TOTAL);
  
  text_init(&menu->options[PMO_RESUME], sub_font, "resume");
  text_center_horizontal(&menu->options[PMO_RESUME], 800);
  menu->options[MMO_PLAY].rect.y = menu->title.rect.y + menu->title.rect.h + 20.f;

  text_init(&menu->options[PMO_MENU], sub_font, "main menu");
  text_center_horizontal(&menu->options[PMO_MENU], 800);
  menu->options[PMO_MENU].rect.y = menu->title.rect.y + menu->title.rect.h + 50.f;

  text_init(&menu->options[PMO_QUIT], sub_font, "quit");
  text_center_horizontal(&menu->options[PMO_QUIT], 800);
  menu->options[MMO_QUIT].rect.y = menu->title.rect.y + menu->title.rect.h + 80.f;
}

void main_menu_update(main_menu *menu, const SDL_Event *event, const game_frame *delta) {
  static int up_press  = 0;
  static int down_press  = 0;

  if (event->type == SDL_KEYDOWN) {
    if (event->key.keysym.scancode == SDL_SCANCODE_UP && up_press == 0) {
      up_press = 1;
      menu->selected++;
    }

    if (event->key.keysym.scancode == SDL_SCANCODE_DOWN && down_press == 0) {
      down_press = 1;
      menu->selected--;
    }
  }

  if (event->type == SDL_KEYUP) {
    if (event->key.keysym.scancode == SDL_SCANCODE_UP && up_press == 1) {
      up_press = 0;
    }

    if (event->key.keysym.scancode == SDL_SCANCODE_DOWN && down_press == 1) {
      down_press = 0;
    }
  }

  if (menu->selected < 0) {
    menu->selected = MMO_TOTAL - 1;
  } else if (menu->selected >= MMO_TOTAL) {
    menu->selected = 0;
  }

  return;
}

void pause_menu_update(pause_menu *menu, const SDL_Event *event, const game_frame *delta) {
  static int up_press  = 0;
  static int down_press  = 0;

  if (event->type == SDL_KEYDOWN) {
    if (event->key.keysym.scancode == SDL_SCANCODE_UP && up_press == 0) {
      up_press = 1;
      menu->selected++;
    }

    if (event->key.keysym.scancode == SDL_SCANCODE_DOWN && down_press == 0) {
      down_press = 1;
      menu->selected--;
    }
  }

  if (event->type == SDL_KEYUP) {
    if (event->key.keysym.scancode == SDL_SCANCODE_UP && up_press == 1) {
      up_press = 0;
    }

    if (event->key.keysym.scancode == SDL_SCANCODE_DOWN && down_press == 1) {
      down_press = 0;
    }
  }

  if (menu->selected < 0) {
    menu->selected = PMO_TOTAL - 1;
  } else if (menu->selected >= PMO_TOTAL) {
    menu->selected = 0;
  }

  return;
}

void main_menu_draw(main_menu *menu) {
  SDL_Color tex_col = { 0xA0, 0x20, 0x80, 0x80};
  SDL_Color sel_col = { 0x20, 0xA0, 0xA0, 0x80};

  render_fill_text(&menu->title, &tex_col);
  for (int i = 0; i < MMO_TOTAL; i++) {
    if (menu->selected == i) {
      render_fill_text(&menu->options[i], &sel_col);
    } else {
      render_fill_text(&menu->options[i], &tex_col);
    }
  }
}

void pause_menu_draw(pause_menu *menu) {
  SDL_Color tex_col = { 0xA0, 0x20, 0x80, 0x80};
  SDL_Color sel_col = { 0x20, 0xA0, 0xA0, 0x80};

  render_fill_text(&menu->title, &tex_col);
  for (int i = 0; i < PMO_TOTAL; i++) {
    if (menu->selected == i) {
      render_fill_text(&menu->options[i], &sel_col);
    } else {
      render_fill_text(&menu->options[i], &tex_col);
    }
  }
}

void main_menu_destroy(main_menu *menu) {
  text_destroy(&menu->title);
  for (int i = 0; i < MMO_TOTAL; i++) {
    text_destroy(&menu->options[i]);
  }

  SDL_free(menu->options);
  menu->options = NULL;
}

void pause_menu_destroy(pause_menu *menu) {
  text_destroy(&menu->title);
  for (int i = 0; i < PMO_TOTAL; i++) {
    text_destroy(&menu->options[i]);
  }

  SDL_free(menu->options);
  menu->options = NULL;
}