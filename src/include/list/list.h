#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

typedef struct list_struct *list;

list list_create(size_t element_size);
void list_destroy(list self);

int list_append(list self, const void *element);
int list_pop(list self, void *element);
int list_insert(list self, const void *element, const size_t index);
int list_remove(list self, size_t index);
int list_get(const list self, void *element, const size_t index);
int list_set(const list self, const void *element, const size_t index);
size_t list_size(const list self);
int list_empty(list self);
int list_isempty(const list self);

#endif