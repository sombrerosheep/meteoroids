#include <resources.h>

#include <SDL.h>

static const char* progyy_font_path = "./data/font/proggy/ProggyCleanSZ.ttf";

void resources_init(resources *r) {
  font_init(&r->fonts[FONT_RESOURCE_PROGGY_TITLE], progyy_font_path, 92);
  font_init(&r->fonts[FONT_RESOURCE_PROGGY_SUB], progyy_font_path, 30);
}

void resources_destroy(resources *r) {
  font_destroy(&r->fonts[FONT_RESOURCE_PROGGY_TITLE]);
  font_destroy(&r->fonts[FONT_RESOURCE_PROGGY_SUB]);
}