#ifndef RESOURCES_H
#define RESOURCES_H

#include <text.h>

enum font_resource_type {
  FONT_RESOURCE_PROGGY_TITLE = 0,
  FONT_RESOURCE_PROGGY_SUB,
  FONT_RESOURCE_SIZE
};

typedef struct resources {
  font fonts[FONT_RESOURCE_SIZE - 1];
} resources;

void resources_init(resources *r);
void resources_destroy(resources *r);

#endif