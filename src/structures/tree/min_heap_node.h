//
// Created by dominik on 12/30/23.
//

#ifndef TRANSMITR_MIN_HEAP_NODE_H
#define TRANSMITR_MIN_HEAP_NODE_H

#include <stdlib.h>

typedef struct MinHeapNode
{
    char data;
    long long freq;
    //struct MinHeapNode* left;
    //struct MinHeapNode* right;
}MinHeapNode;

void min_heap_node_init(MinHeapNode* this, char data, long long freq);
long long min_heap_node_get_freq(MinHeapNode* this);
void min_heap_node_destroy(MinHeapNode* this);


#endif // TRANSMITR_MIN_HEAP_NODE_H
