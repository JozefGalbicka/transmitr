//
// Created by dominik on 12/31/23.
//

#ifndef TRANSMITR_HUFFMAN_TREE_H
#define TRANSMITR_HUFFMAN_TREE_H

#include "structures/tree/min_heap.h"
#include "frequency_table.h"
#include "code_table.h"

typedef struct HuffmanTree
{
    MinHeap* minHeap;
    FrTable* frTable;
}HuffmanTree;

void huffman_tree_init(HuffmanTree* this, MinHeap* minHeap);
void huffman_tree_destroy(HuffmanTree* this);
void huffman_tree_create(HuffmanTree* this);
size_t huffman_tree_size(HuffmanTree* this);
MinHeapNode* huffman_tree_get_root(HuffmanTree* this);
void huffman_tree_generate_code_for_node(MinHeapNode * node, char* code, int length, CodeTable* table);
void huffman_tree_generate_codes(HuffmanTree* tree, CodeTable* table);



#endif // TRANSMITR_HUFFMAN_TREE_H
