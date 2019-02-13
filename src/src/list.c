#include "list/list.h"
#include <string.h>
#include <stdio.h>

typedef struct node_
{
    struct node_ *next;
    struct node_ *prev;
    void *data;
} node;

struct list_struct
{
    size_t element_size;
    node *head;
    node *tail;
    size_t size;
};

struct list_iterator
{
    iterator iter;
    list the_list;
    node *current;
};

static node *alloc_node(const void *data, const size_t element_size);
static node *free_node(node *self);
static void list_iterator_first(iterator *it);
static void list_iterator_last(iterator *it);
static void list_iterator_next(iterator *it);
static void list_iterator_prev(iterator *it);
static int list_iterator_is_end(iterator *it);
static void list_iterator_current(iterator *it, void *node);

static struct iterator_ops list_it_ops = {
        .first = list_iterator_first,
        .last = list_iterator_last,
        .next = list_iterator_next,
        .prev = list_iterator_prev,
        .is_end = list_iterator_is_end,
        .current = list_iterator_current
};

list list_create(size_t element_size)
{
    list self = malloc(sizeof(*self));

    if (self) {
        self->element_size = element_size;
        self->head = NULL;
        self->tail = NULL;
        self->size = 0;
    }

    return self;
}

void list_destroy(list self)
{
    if (self) {
        while (self->head != NULL) {
            self->head = free_node(self->head);
        }
    }
    free(self);
}

iterator *list_create_iterator(list self)
{
    struct list_iterator *iter = malloc(sizeof(*iter));

    if (self && iter) {
        iterator_init(&iter->iter);
        iter->iter.ops = &list_it_ops;
        iter->the_list = self;
        iter->current = self->head;
    }

    // Nasty bad casting. boooo. Or you could return &iter->iter since they're the same address
    return (iterator*)iter;
}

int list_append(list self, const void *element)
{
    int error = -1;
    node *new_node = NULL;

    if (self) {
        new_node = alloc_node(element, self->element_size);
        if (new_node) {
            // This is the first element of the list
            if (self->tail == NULL) {
                self->head = new_node;
                self->tail = new_node;
            } else {
                self->tail->next = new_node;
                new_node->prev = self->tail;
                self->tail = new_node;
            }
            self->size++;
            error = 0;
        } else
        {
            error = -2;
        }
    }

    return error;
}

int list_pop(list self, void *element)
{
    int error = -1;

    if (self) {
        if (self->head) {
            memcpy(element, self->head->data, self->element_size);
            self->head = free_node(self->head);
            self->size--;
            error = 0;
        } else {
            error = -2;
        }
    }

    return error;
}

int list_insert(list self, const void *element, const size_t index)
{
    int error = -1;
    size_t i;
    node *iter_node = NULL;
    node *new_node = NULL;

    if (self) {
        // We can only insert up to +1 of the current highest index
        if (index > self->size) {
            error = -2;
        } else {
            iter_node = self->head;
            new_node = alloc_node(element, self->element_size);
            if (iter_node) {
                for (i = 0; i < index; i++) {
                    if (iter_node) {
                        iter_node = iter_node->next;
                    }
                }
                if (iter_node) {
                    // Insert operation
                    new_node->next = iter_node;
                    new_node->prev = iter_node->prev;

                    if (new_node->prev) {
                        new_node->prev->next = new_node;
                    }

                    iter_node->prev = new_node;
                    if (index == 0) {
                        self->head = new_node;
                    }
                } else {
                    // Appending to list
                    self->tail->next = new_node;
                    new_node->prev = self->tail;
                    self->tail = new_node;
                }
            } else {
                // Insert at head
                self->head = new_node;
                self->tail = new_node;
            }

            self->size++;
            error = 0;
        }
    }

    return error;
}

int list_remove(list self, size_t index)
{
    int error = -1;
    size_t i;
    node *iter_node = NULL;

    if (self) {
        if (index >= self->size) {
            error = -2;
        }
        iter_node = self->head;
        for (i = 0; i < index; i++) {
            if (iter_node) {
                iter_node = iter_node->next;
            }
        }
        if (iter_node) {
            if (iter_node != self->head) {
                iter_node->prev->next = iter_node->next;
            } else {
                self->head = iter_node->next;
            }

            if (iter_node != self->tail) {
                iter_node->next->prev = iter_node->prev;
            } else {
                self->tail = iter_node->prev;
            }
            free_node(iter_node);
            self->size--;
            error = 0;
        } else {
            error = -3;
        }
    }

    return error;
}

int list_get(const list self, void *element, const size_t index)
{
    int error = -1;
    size_t i;
    node *iter_node = NULL;

    if (self) {
        // Accessing outside list is undefined
        if (index >= self->size) {
            error = -2;
        } else {
            iter_node = self->head;
            for (i = 0; i < index; i++) {
                if (iter_node) {
                    iter_node = iter_node->next;
                }
            }
            memcpy(element, iter_node->data, self->element_size);
            error = 0;
        }
    }

    return error;
}

int list_set(const list self, const void *element, const size_t index)
{
    int error = -1;
    size_t i;
    node *iter_node = NULL;

    if (self) {
        // Accessing outside list is undefined
        if (index >= self->size) {
            error = -2;
        } else {
            iter_node = self->head;
            for (i = 0; i < index; i++) {
                if (iter_node) {
                    iter_node = iter_node->next;
                }
            }
            memcpy(iter_node->data, element, self->element_size);
            error = 0;
        }
    }

    return error;
}

size_t list_size(const list self)
{
    if (self) {
        return self->size;
    } else {
        return 0;
    }
}

int list_empty(list self)
{
    int error = -1;

    if (self) {
        while (self->head != NULL) {
            self->head = free_node(self->head);
        }
        self->size = 0;
        self->tail = NULL;
        error = 0;
    }

    return error;
}

int list_isempty(const list self)
{
    int empty = 1;
    if (self) {
        if (self->size) {
            empty = 0;
        }
    }

    return empty;
}

int list_find_index(const list self, const void *element, int (*cmp)(const void *, const void*))
{
    int i;
    int index = -2;
    size_t size;
    void *cmp_element = malloc(self->element_size);

    if (self && element && cmp_element) {
        size = list_size(self);
        for(i = 0; i < size; i++) {
            list_get(self, cmp_element, i);
            if (cmp(element, cmp_element)) {
                index = i;
                break;
            }
        }
    } else {
        index = -1;
    }

    free(cmp_element);
    return index;
}

static node *alloc_node(const void *data, const size_t element_size)
{
    node *self = malloc(sizeof(*self));
    if (self) {
        self->data = malloc(element_size);
        if (self->data) {
            memcpy(self->data, data, element_size);
            self->next = NULL;
            self->prev = NULL;
        } else {
            free(self);
            self = NULL;
        }
    }

    return self;
}

static node *free_node(node *self)
{
    node *next = NULL;
    if (self) {
        next = self->next;
        free(self->data);
        free(self);
    }

    return next;
}

static void list_iterator_first(iterator *it_old)
{
    struct list_iterator *it = (struct list_iterator *)it_old;

    if (it) {
        it->current = it->the_list->head;
    }
}

static void list_iterator_last(iterator *it_old)
{
    struct list_iterator *it = (struct list_iterator *)it_old;

    if (it) {
        it->current = it->the_list->tail;
    }
}

static void list_iterator_next(iterator *it_old)
{
    struct list_iterator *it = (struct list_iterator *)it_old;
    if (it) {
        if (it->current) {
            it->current = it->current->next;
        }
    }
}

static void list_iterator_prev(iterator *it_old)
{
    struct list_iterator *it = (struct list_iterator*)it_old;
    if (it) {
        if (it->current) {
            it->current = it->current->prev;
        }
    }
}

static int list_iterator_is_end(iterator *it_old)
{
    int is_end = -1;
    struct list_iterator *it = (struct list_iterator *)it_old;

    if (it) {
        if (it->current) {
            is_end = 0;
        } else {
            is_end = 1;
        }
    }

    return is_end;
}

static void list_iterator_current(iterator *it_old, void *node)
{
    struct list_iterator *it = (struct list_iterator *)it_old;
    if (it) {
        if (it->current) {
            memcpy(node, it->current->data, it->the_list->element_size);
        }
    }
}
