//
// Created by dominik on 12/30/23.
//

#ifndef TRANSMITR_MIN_HEAP_NODE_H
#define TRANSMITR_MIN_HEAP_NODE_H

#include <stdlib.h>

typedef struct MinHeapNode
{
    unsigned char data;
    long long freq;
    struct MinHeapNode* left;
    struct MinHeapNode* right;
}MinHeapNode;

void min_heap_node_init(MinHeapNode* this, unsigned char data, long long freq);
long long min_heap_node_get_freq(MinHeapNode* this);
unsigned char min_heap_node_get_data(MinHeapNode* this);
void min_heap_node_set_freq(MinHeapNode* this, long long freq);
void min_heap_node_set_data(MinHeapNode* this, unsigned char data);
void min_heap_node_destroy(MinHeapNode* this);
void min_heap_node_set_right(MinHeapNode* this, MinHeapNode* right);
void min_heap_node_set_left(MinHeapNode* this, MinHeapNode* left);
MinHeapNode* min_heap_node_get_right(MinHeapNode* this);
MinHeapNode* min_heap_node_get_left(MinHeapNode* this);
void huffman_tree_free(MinHeapNode* node);

#endif // TRANSMITR_MIN_HEAP_NODE_H
