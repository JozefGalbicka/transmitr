//
// Created by dominik on 12/29/23.
//

#include "min_heap.h"
#include <stdlib.h>
#include <string.h>

#define DEFAULT_CAPACITY 10
#define EMPTY 0


void min_heap_init(MinHeap* this, size_t node_size)
{
    this->node_size = node_size;
    this->capacity = DEFAULT_CAPACITY;
    this->size = EMPTY;
    this->array = calloc(DEFAULT_CAPACITY,node_size);
}


void min_heap_destroy(MinHeap* this, void (*destroy_node)(void* node))
{
    for(int i = 0; i < this->size; i++)
    {
        destroy_node((unsigned char*)this->array + i*this->node_size);
    }

    free(this->array);
    this->capacity = EMPTY;
    this->size = EMPTY;
    this->node_size = EMPTY;
    this->array = NULL;
}


static bool min_heap_has_capacity(MinHeap* this)
{
    return this->size != this->capacity;
}


static bool min_heap_increase_capacity(MinHeap* this)
{
    this->capacity *= 2;
    void* tmp = realloc(this->array, this->capacity*this->node_size);

    if( tmp == NULL)
        return false;
    else
    {
        this->array = tmp;
        return true;
    }
}


int min_heap_get_parent(int index)
{
    if(index <= 0)
        return -42;

    return (index - 1) / 2 ;
}


bool min_heap_add(MinHeap* this,unsigned char data, long long freq,long long (*get_Value)(void*),void (*init)(void*,unsigned char,long long), void (*destroy)(void*))
{
    void* new = malloc(this->node_size);

    if (new == NULL)
        return false;

    init(new, data, freq);

    if(!min_heap_has_capacity(this))
        if(!min_heap_increase_capacity(this))
            return false;

    if(this->size == EMPTY)
    {
        memcpy((unsigned char*)this->array,new,this->node_size);
        this->size++;
        free(new);
        return true;
    }

    size_t i = this->size;
    this->size++;

    while (i > 0) {
        size_t parentIndex = min_heap_get_parent(i);
        if (get_Value(new) >= get_Value((void*)((unsigned char*)this->array + parentIndex * this->node_size))) {
            break;
        }

        memcpy((unsigned char*)this->array + i * this->node_size,(unsigned char*)this->array + parentIndex * this->node_size,this->node_size);
        i = parentIndex;
    }

    memcpy((unsigned char*)this->array + i * this->node_size, new, this->node_size);

    destroy(new);
    free(new);
    return true;
}


size_t min_heap_size(MinHeap* this)
{
    return this->size;
}


void* min_heap_get_root(MinHeap* this)
{
    if (this->size == 0)
        return NULL;

    return this->array;
}


int min_heap_get_right_child(int index)
{
    return (2*index + 2);
}


int min_heap_get_left_child(int index)
{
    return (2*index + 1);
}


void min_heap_remove_min(MinHeap* this,long long (*get_Value)(void*), void* output)
{
    if(this->size == EMPTY)
    {
        fprintf(stderr,"Empty heap!!");
        return;
    }

    if(this->size == 1)
    {
        this->size--;
        memcpy(output,this->array,this->node_size);
        return;
    }

    memcpy(output,this->array,this->node_size);
    this->size--;
    memcpy(this->array,(unsigned char*)this->array + this->size*this->node_size,this->node_size);



    int start_index = 0;
    while (start_index < this->size)
    {
        int min_index = start_index;
        int left = min_heap_get_left_child(start_index);
        int right = min_heap_get_right_child(start_index);

        if (left < this->size && get_Value((unsigned char*)this->array + left*this->node_size) < get_Value((unsigned char*)this->array + min_index*this->node_size))
            min_index = left;

        if (right < this->size && get_Value((unsigned char*)this->array + right*this->node_size) < get_Value((unsigned char*)this->array + min_index*this->node_size))
            min_index = right;

        if (min_index == start_index)
            break;

        void* tmp = malloc(this->node_size);
        memcpy(tmp,this->array + start_index*this->node_size,this->node_size);
        memcpy(this->array + start_index*this->node_size,this->array + min_index*this->node_size,this->node_size);
        memcpy(this->array + min_index*this->node_size,tmp,this->node_size);

        free(tmp);
        start_index = min_index;
    }
}

bool min_heap_add_node(MinHeap* this, void* node,long long (*get_Value)(void*))
{
    if (this == NULL || node == NULL)
        return false;

    if (!min_heap_has_capacity(this))
        if (!min_heap_increase_capacity(this))
            return false;

    size_t i = this->size;
    this->size++;
    while (i > 0)
    {
        size_t parentIndex = min_heap_get_parent(i);
        if (get_Value(node) >= get_Value((void*)((unsigned char*)this->array + parentIndex * this->node_size)))
            break;

        memcpy((unsigned char*)this->array + i * this->node_size,(unsigned char*)this->array + parentIndex * this->node_size,this->node_size);
        i = parentIndex;
    }

    memcpy((unsigned char*)this->array + i * this->node_size, node, this->node_size);
    return true;
}
