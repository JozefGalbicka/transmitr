//
// Created by dominik on 12/30/23.
//
#define EMPTY 0
#include "min_heap_node.h"

void min_heap_node_init(MinHeapNode* this, unsigned char data, long long freq)
{
    this->data = data;
    this->freq = freq;
    this->right = NULL;
    this->left = NULL;
}


long long min_heap_node_get_freq(MinHeapNode* this)
{
    return this->freq;
}


unsigned char min_heap_node_get_data(MinHeapNode* this)
{
    return this->data;
}


void min_heap_node_set_freq(MinHeapNode* this, long long freq)
{
    this->freq = freq;
}


void min_heap_node_set_data(MinHeapNode* this, unsigned char data)
{
    this->data = data;
}


void min_heap_node_destroy(MinHeapNode* this)
{
    this->freq = EMPTY;
    this->data = EMPTY;
}


void min_heap_node_set_right(MinHeapNode* this, MinHeapNode* right)
{
    this->right = right;
}


void min_heap_node_set_left(MinHeapNode* this, MinHeapNode* left)
{
    this->left = left;
}


MinHeapNode* min_heap_node_get_right(MinHeapNode* this)
{
    return this->right;
}


MinHeapNode* min_heap_node_get_left(MinHeapNode* this)
{
    return this->left;
}

