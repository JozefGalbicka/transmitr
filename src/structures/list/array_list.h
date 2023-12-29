
#include <stddef.h>
#include <stdio.h>

typedef struct ArrayList {
    void *array;
    size_t item_size;
    size_t size;
    size_t capacity;
} ArrayList;

typedef struct ArrayListIterator {
    ArrayList *list;
    size_t index;
} ArrayListIterator;

// Base operations
void array_list_init(ArrayList *self, size_t item_size);
void array_list_destroy(ArrayList *self);
// void array_list_clear(ARRAY_LIST *self); // Not implemented

// Data operations
_Bool array_list_get_at(ArrayList *self, size_t index, void *item);
void array_list_add(ArrayList *self, const void *item);
_Bool array_list_insert_at(ArrayList *self, size_t index, const void *item);
_Bool array_list_set_at(ArrayList *self, size_t index, const void *item);
_Bool array_list_remove_at(ArrayList *self, size_t index, void *item);
_Bool array_list_assign(ArrayList *self, ArrayList *other);
_Bool array_list_equals(ArrayList *self, ArrayList *other);

// File operations
_Bool array_list_save_to_file(ArrayList *self, const char *file_name);
_Bool array_list_read_from_file(ArrayList *self, const char *file_name);
void array_list_to_txt(ArrayList *self, FILE *f,
                       void (*item_to_file)(void *item, FILE *f));

// Sorting operations
void array_list_bubblesort(ArrayList *self,
                           int (*cmp)(const void *itemA, const void *itemB));
void array_list_qsort(ArrayList *self, int (*cmp)(const void *, const void *));

// Iterators
void array_list_iterator_init(ArrayListIterator *self, ArrayList *list);
void array_list_iterator_destroy(ArrayListIterator *self);
_Bool array_list_iterator_has_next(ArrayListIterator *self);
void *array_list_iterator_move_next(ArrayListIterator *self);
