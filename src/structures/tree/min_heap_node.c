//
// Created by dominik on 12/30/23.
//
#define EMPTY 0
#include "min_heap_node.h"

void min_heap_node_init(MinHeapNode* this, char data, long long freq)
{
    this->data = data;
    this->freq = freq;
}


long long min_heap_node_get_freq(MinHeapNode* this)
{
    return this->freq;
}


void min_heap_node_destroy(MinHeapNode* this)
{
    this->freq = EMPTY;
    this->data = EMPTY;
}