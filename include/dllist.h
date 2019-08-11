#ifndef DLLIST_H
#define DLLIST_H

typedef struct dllist_element {
  void *data;
  struct dllist_element *next;
  struct dllist_element *prev;
} dllist_element;

typedef struct dllist {
  int                         size;
  dllist_element              *head;
  dllist_element              *tail;
  void (*destroy)(void *data);
} dllist;

int dllist_init(dllist *list, void (*destroy)(void *data));
int dllist_ins(dllist *list, void *data);
int dllist_rem(dllist *list, void **data);
void dllist_destroy(dllist *list);

#endif