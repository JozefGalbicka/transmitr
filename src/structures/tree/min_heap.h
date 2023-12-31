//
// Created by dominik on 12/29/23.
//

#ifndef TRANSMITR_MIN_HEAP_H
#define TRANSMITR_MIN_HEAP_H

#include <stdio.h>
#include <stdbool.h>
#include "min_heap_node.h"

typedef struct MinHeap
{
    void* array;
    size_t node_size;
    size_t capacity;
    size_t size;
}MinHeap;

void min_heap_init(MinHeap* this, size_t node_size);
void min_heap_destroy(MinHeap* this, void (*destroy_node)(void* node));

bool min_heap_add(MinHeap* this,char data, long long freq,long long (*get_Value)(void*));


size_t min_heap_size(MinHeap* this);
void* min_heap_get_root(MinHeap* this);
int min_heap_get_parent(int index);
int min_heap_get_right_child(int index);
int min_heap_get_left_child(int index);

void min_heap_remove_min(MinHeap* this,long long (*get_Value)(void*),void* output);









#endif // TRANSMITR_MIN_HEAP_H
