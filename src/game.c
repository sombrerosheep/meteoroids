#include <game.h>

#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <player.h>
#include <renderer.h>
#include <dllist.h>
#include <meteoroid.h>
#include <game_clock.h>
#include <bullet.h>
#include <time.h>
#include <text.h>

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800

#define NUM_STARTING_METEOROIDS 8
#define PLAYER_START_X 400.f
#define PLAYER_START_Y 300.f

void keep_player_in_bounds(game_context *ctx) {
  SDL_Rect viewport;
  vec2f offset;
  rectf aabb = shape_aabb(&ctx->state->player->sprite);
  aabb.x += ctx->state->player->pos.x;
  aabb.y += ctx->state->player->pos.y;

  renderer_get_viewport(&viewport);
  if (rectf_outersects(&aabb, &viewport, &offset) == SDL_TRUE) {
    ctx->state->player->pos.x += offset.x;
    ctx->state->player->pos.y += offset.y;
  }
}

void destroy_meteoroid(void *data) {
  meteoroid *m;

  m = (meteoroid*)data;

  SDL_free(m);
}

void meteoroid_bullets_collisions(game_state *state, meteoroid *m) {
  dllist *bullets = &state->player->bullets;
  dllist_element *e;

  for (e = bullets->head; e != NULL; e = e->next) {
    bullet *b;
    
    b = (bullet*)e->data;
    if (rectf_intersects_rectf(&b->sprite, &m->sprite) == SDL_TRUE) {
      m->health -= b->health;
      b->health = 0;
    }
  }
}

void resolve_meteoroid_collisions(meteoroid *m, dllist *list) {
  for (dllist_element *e = list->head; e != NULL; e = e->next) {
    meteoroid *em = e->data;
    rectf intersect = { 0.f, 0.f, 0.f, 0.f };

    if (em == m) {
      continue;
    }

    // back off the intersection amount and then set the velocity
    if (rectf_intersection(&m->sprite, &em->sprite, &intersect) == SDL_TRUE) {
      if (intersect.h > intersect.w) {
        // assume that hit was left or right
        m->velocity.x *= -1.f;
        if (intersect.x > m->sprite.x) {
          // collision was on right
          m->sprite.x -= intersect.w;
        } else {
          // collision was on left
          m->sprite.x += intersect.w;
        }
      } else {
        // assume that hit was top or bottom
        m->velocity.y *= -1.f;
        if (intersect.y > m->sprite.y) {
          // collision was on bottom
          m->sprite.y -= intersect.h;
        } else {
          // collision was on top
          m->sprite.y += intersect.h;
        }
      }

      return;
    }
  }
}

void update_player_collisions(Player *p, dllist *meteoroids) {
  shape player_global;
  shape_init(&player_global, p->sprite.points, p->sprite.num_points);
  shape_rotate(&player_global, p->rotation);
  shape_translate(&player_global, p->pos);

  for (int i = 0; i < player_global.num_points; i++) {
    for (dllist_element *e = meteoroids->head; e != NULL; e = e->next) {
      meteoroid *m = e->data;
      if (rectf_contains_vec2f(&m->sprite, &player_global.points[i]) == SDL_TRUE) {
        p->alive = SDL_FALSE;
        return;
      }
    }
  }
}

void update_meteoroids(game_state *state, game_frame *delta) {
  dllist_element *e;
  meteoroid *m;
  SDL_Rect viewport;

  renderer_get_viewport(&viewport);

  for (e = state->meteoroids->head; e != NULL;) {
    m = (meteoroid*)e->data;

    meteoroid_bullets_collisions(state, m);
    meteoroid_update(m, delta);
    keep_in_bounds(&m->sprite, &viewport);

    if (m->health < 0) {
      // handle generations
      dllist_element *prev = e->prev;
      dllist_rem(state->meteoroids, (void**)&m);
      meteoroid_seed_next_generation(state->meteoroids, m->gen, m->sprite.x, m->sprite.y);
      destroy_meteoroid(m);
      e = prev;
    } else {
      e = e->next;
    }

    resolve_meteoroid_collisions(m, state->meteoroids);
  }
}

void draw_meteoroids(dllist *l) {
  dllist_element *e;

  for (e = l->head; e != NULL; e = e->next) {
    meteoroid_draw((meteoroid*)e->data);
  }
}

void build_game_world(game_state *state) {
  float x, y, v_x, v_y;
  float safe_zone;
  int intersects;
  SDL_Rect viewport;
  meteoroid *m;

  safe_zone = 100.f;
  renderer_get_viewport(&viewport);

  for (int i = 0; i < NUM_STARTING_METEOROIDS; i++) {
    m = SDL_malloc(sizeof(meteoroid));
    meteoroid_init(m, FIRST);

    x = random_getf_between((float)viewport.x, (float)viewport.w);
    y = random_getf_between((float)viewport.y, (float)viewport.h);
    v_x = random_getf_between(-METEOROID_VELOCITY_RANGE, METEOROID_VELOCITY_RANGE);
    v_y = random_getf_between(-METEOROID_VELOCITY_RANGE, METEOROID_VELOCITY_RANGE);

    m->velocity.x = v_x;
    m->velocity.y = v_y;
    m->sprite.x = x;
    m->sprite.y = y;

    intersects = rectf_intersects_circle(&m->sprite, 400.f, 300.f, safe_zone);
    if (intersects != -1) {
      switch (intersects) {
        case 0:
          m->sprite.x += 100.f;
          m->sprite.y += 100.f;
          break;
        case 1:
          m->sprite.x -= (100.f + 30.f);
          m->sprite.y += (100.f + 30.f);
          break;
        case 2:
          m->sprite.x += (100.f + 30.f);
          m->sprite.y -= (100.f + 30.f);
          break;
        case 3:
          m->sprite.x -= (100.f + 30.f);
          m->sprite.y -= (100.f + 30.f);
          break;
        default:
          break;
      }
    }

    dllist_ins(state->meteoroids, m);
  }
}

void seed_game_world(game_state *state) {
  player_init(state->player, PLAYER_START_X, PLAYER_START_Y);
  dllist_init(state->meteoroids, destroy_meteoroid);

  build_game_world(state);
}

void game_update(game_context *ctx, SDL_Event *event, game_frame *delta) {
  game_input input;
  input = game_input_state(ctx->state->bindings);

  switch (ctx->state->mode) {
    case GAME_MODE_MENU: {
      if (event->type == SDL_KEYDOWN && event->key.keysym.scancode == SDL_SCANCODE_RETURN) {
        if (ctx->state->main_menu->selected == 0) {
          dllist_destroy(ctx->state->meteoroids);
          player_destroy(ctx->state->player);
          seed_game_world(ctx->state);
          ctx->state->mode = GAME_MODE_PLAY;
        }
        if (ctx->state->main_menu->selected == 1) {
          ctx->running = 0;
        }
      }

      main_menu_update(ctx->state->main_menu, event, delta);
      update_meteoroids(ctx->state, delta);

      break;
    }
    case GAME_MODE_PAUSE: {
      if (event->type == SDL_KEYDOWN && event->key.keysym.scancode == SDL_SCANCODE_RETURN) {
        ctx->state->mode = GAME_MODE_PLAY;
      }

      pause_menu_update(ctx->state->pause_menu, event, delta);

      break;
    }
    case GAME_MODE_PLAY: {
      if (ctx->state->player->alive == SDL_FALSE && event->type == SDL_KEYDOWN && event->key.keysym.scancode == SDL_SCANCODE_R) {
        dllist_destroy(ctx->state->meteoroids);
        player_destroy(ctx->state->player);
        seed_game_world(ctx->state);
      }

      if (ctx->state->player->alive && event->type == SDL_KEYDOWN && event->key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
        ctx->state->mode = GAME_MODE_PAUSE;
      }

      player_update(ctx->state->player, &input, delta);
      update_meteoroids(ctx->state, delta);
      update_player_collisions(ctx->state->player, ctx->state->meteoroids);

      keep_player_in_bounds(ctx);

      break;
    }
  }

  return;
}

void game_draw(game_context *ctx) {
  SDL_SetRenderDrawColor(ctx->renderer, 0x0, 0x0, 0x0, 0xFF);
  SDL_RenderClear(ctx->renderer);
  SDL_Color tex_col = { 0xA0, 0x20, 0x80, 0x80};

  switch (ctx->state->mode) {
    case GAME_MODE_MENU: {
      main_menu_draw(ctx->state->main_menu);
      draw_meteoroids(ctx->state->meteoroids);

      break;
    }
    case GAME_MODE_PAUSE: {
      pause_menu_draw(ctx->state->pause_menu);

      break;
    }
    case GAME_MODE_PLAY: {
      if (!ctx->state->player->alive) {
        render_fill_text(ctx->state->dead_text, &tex_col);
        render_fill_text(ctx->state->dead_desc_text, &tex_col);
      }

      player_draw(ctx->state->player);
      draw_meteoroids(ctx->state->meteoroids);

      break;
    }
  }

  SDL_RenderPresent(ctx->renderer);
}

int game_init_systems(game_context *ctx) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("Error initializing SDL\n");
    return -1;
  }

  if ((ctx->window = SDL_CreateWindow("Meteoroids", 200, 125, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL)) == NULL) {
    printf("Error creating window %s\n", SDL_GetError());
    return -1;
  }

  if ((ctx->renderer = SDL_CreateRenderer(ctx->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) == NULL) {
    printf("Error creating renderer %s\n", SDL_GetError());
    return -1;
  }

  if (TTF_Init() != 0) {
    printf("Error initializing sdl_ttf: %s\n", TTF_GetError());
    return -1;
  }

  return 0;
}

int game_init(game_context *ctx, game_key_bindings *key_bindings) {
  if (!SDL_VERSION_ATLEAST(2, 0, 10)) {
    SDL_version ver;
    SDL_GetVersion(&ver);
    printf("Minimum SDL Version not met.\n\tWant 2.10.0+.\n\tHave: %d.%d.%d\n", ver.major, ver.minor, ver.patch);
    return -1;
  }

  random_init(clock());

  if (game_init_systems(ctx) != 0) {
    printf("Error detected during init...exiting");
    return -1;
  }

  ctx->resources = SDL_malloc(sizeof(resources));
  resources_init(ctx->resources);

  ctx->state = SDL_malloc(sizeof(game_state));
  ctx->state->player = SDL_malloc(sizeof(Player));
  ctx->state->meteoroids = SDL_malloc(sizeof(dllist));
  ctx->state->bindings = key_bindings;
  ctx->state->dead_text = SDL_malloc(sizeof(text));
  ctx->state->dead_desc_text = SDL_malloc(sizeof(text));
  ctx->state->main_menu = SDL_malloc(sizeof(main_menu));
  ctx->state->pause_menu = SDL_malloc(sizeof(pause_menu));

  text_init(ctx->state->dead_text, &ctx->resources->fonts[FONT_RESOURCE_PROGGY_TITLE], "you died");
  text_center(ctx->state->dead_text, WINDOW_HEIGHT - 100, WINDOW_WIDTH);
  text_init(ctx->state->dead_desc_text, &ctx->resources->fonts[FONT_RESOURCE_PROGGY_SUB], "press <r> to restart");
  text_center_horizontal(ctx->state->dead_desc_text, WINDOW_WIDTH);
  ctx->state->dead_desc_text->rect.y = ctx->state->dead_text->rect.y + ctx->state->dead_text->rect.h + 20.f;

  main_menu_init(ctx->state->main_menu, &ctx->resources->fonts[FONT_RESOURCE_PROGGY_TITLE], &ctx->resources->fonts[FONT_RESOURCE_PROGGY_SUB]);
  pause_menu_init(ctx->state->pause_menu, &ctx->resources->fonts[FONT_RESOURCE_PROGGY_TITLE], &ctx->resources->fonts[FONT_RESOURCE_PROGGY_SUB]);

  ctx->state->mode = GAME_MODE_MENU;

  renderer_set(ctx->renderer);

  seed_game_world(ctx->state);

  ctx->running = 1;

  return 0;
}

void game_start(game_context *ctx) {
  SDL_Event event;
  SDL_bool running;
  game_clock clock;
  game_frame delta;

  game_clock_init(&clock);

  while (ctx->running) {
    delta = game_clock_reset(&clock);
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        ctx->running = 0;
      }
    }

    game_update(ctx, &event, &delta);
    game_draw(ctx);
  }
}

void game_destroy_font(font *f) {
  font_destroy(f);
  SDL_free(f);
  f = NULL;
}

void game_destroy_text(text *t) {
  text_destroy(t);
  SDL_free(t);
  t = NULL;
}

void game_destroy(game_context *ctx) {
  ctx->state->bindings = NULL;
  
  player_destroy(ctx->state->player);
  SDL_free(ctx->state->player);
  ctx->state->player = NULL;
  
  dllist_destroy(ctx->state->meteoroids);
  SDL_free(ctx->state->meteoroids);
  ctx->state->player = NULL;

  main_menu_draw(ctx->state->main_menu);
  pause_menu_draw(ctx->state->pause_menu);
  game_destroy_text(ctx->state->dead_text);
  game_destroy_text(ctx->state->dead_desc_text);
  
  SDL_free(ctx->state);
  ctx->state = NULL;

  resources_destroy(ctx->resources);

  if (TTF_WasInit() == 1) {
    TTF_Quit();
  }

  SDL_DestroyRenderer(ctx->renderer);
  SDL_DestroyWindow(ctx->window);
}