#include <stdlib.h>
#include <string.h>
#include <dllist.h>

int dllist_init(dllist *list, void (*destroy)(void *data)) {
  list->size = 0;
  list->destroy = destroy;
  list->head = NULL;
  list->tail = NULL;
}

int dllist_ins(dllist *list, void *data) {
  dllist_element *element;

  if ((element = malloc(sizeof(dllist_element))) == NULL) {
    return -1;
  }

  element->data = (void*)data;
  element->next = NULL;

  if (list->head == NULL) {
    list->head = element;
    list->tail = element;
    element->prev = NULL;
  } else {
    list->tail->next = element;
    element->prev = list->tail;
    list->tail = element;
  }

  list->size++;
  return 0;
}

int dllist_rem(dllist *list, void **data) {
  dllist_element *element;

  if (*data == NULL) {
    return -1;
  }

  for (element = list->head; element != NULL; element = element->next) {
    if (element->data == *data) {
      *data = element->data;
      element->data = NULL;
      
      if (element->prev == NULL) {
        // removing the head
        list->head = element->next;
      } else {
        element->prev->next = element->next;
      }

      if (element->next == NULL) {
        // removing tail
        list->tail = element->prev;
      } else {
        element->next->prev = element->prev;
      }

      free(element);
      list->size--;
      return 0;
    }
  }

  return -1;
}

void dllist_destroy(dllist *list) {
  dllist_element *element;

  while (list->size > 0) {
    element = list->head->data;
    dllist_rem(list, (void**)&element);

    if (list->destroy != NULL) {
      list->destroy(element);
    }
  }

  list->destroy = NULL;

  memset(list, 0, sizeof(dllist));
}
