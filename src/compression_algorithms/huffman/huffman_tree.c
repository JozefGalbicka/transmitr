//
// Created by dominik on 12/31/23.
//
#include "huffman_tree.h"
#include <string.h>
#define MAX_SYMBOLS 256

static long long wrapper_min_heap_node_get_freq(void* node)
{
    return min_heap_node_get_freq((MinHeapNode*) node);
}

static void wrapper_min_heap_node_destroy(void* node)
{
    return min_heap_node_destroy((MinHeapNode*) node);
}

void huffman_tree_init(HuffmanTree* this, MinHeap* minHeap )
{
    this->minHeap = malloc(sizeof(MinHeap));
    this->minHeap->array = malloc(minHeap->node_size * minHeap->capacity);
    memcpy(this->minHeap->array, minHeap->array, minHeap->node_size * minHeap->size);
    this->minHeap->node_size = minHeap->node_size;
    this->minHeap->capacity = minHeap->capacity;
    this->minHeap->size = minHeap->size;
}

void huffman_tree_destroy(HuffmanTree* this)
{
    huffman_tree_free(min_heap_get_root(this->minHeap));
    free(this->minHeap);
    this->minHeap = NULL;
    this->frTable = NULL;
}


void huffman_tree_create(HuffmanTree* this) {

    if (min_heap_size(this->minHeap) == 1)
    {
        MinHeapNode* onlyNode = malloc(sizeof(MinHeapNode));
        min_heap_remove_min(this->minHeap,wrapper_min_heap_node_get_freq,onlyNode);

        MinHeapNode *root = malloc(sizeof(MinHeapNode));
        min_heap_node_init(root, '\0', min_heap_node_get_freq(onlyNode));
        root->left = onlyNode;
        root->right = NULL;

        min_heap_add_node(this->minHeap, root, wrapper_min_heap_node_get_freq);
        free(root);
    }

    while (min_heap_size(this->minHeap) > 1) {
        MinHeapNode *left = malloc(sizeof(MinHeapNode));
        MinHeapNode *right = malloc(sizeof(MinHeapNode));

        min_heap_remove_min(this->minHeap, wrapper_min_heap_node_get_freq,left);
        min_heap_remove_min(this->minHeap, wrapper_min_heap_node_get_freq,right);

        MinHeapNode *top = malloc(sizeof(MinHeapNode));
        min_heap_node_init(top, ' ', min_heap_node_get_freq(left) + min_heap_node_get_freq(right));
        min_heap_node_set_left(top, left);
        min_heap_node_set_right(top, right);

        min_heap_add_node(this->minHeap, top, wrapper_min_heap_node_get_freq);
        free(top);
    }
}



MinHeapNode* huffman_tree_get_root(HuffmanTree* this)
{
    return min_heap_get_root(this->minHeap);
}


size_t huffman_tree_size(HuffmanTree* this)
{
    return min_heap_size(this->minHeap);
}

void huffman_tree_free(MinHeapNode* node) {
    if (node == NULL)
        return;

    huffman_tree_free(min_heap_node_get_left(node));
    huffman_tree_free(min_heap_node_get_right(node));
    free(node);
}


void huffman_tree_generate_code_for_node(MinHeapNode* node, char* code, int length, CodeTable* table)
{
    if (node == NULL)
        return;

    if (!min_heap_node_get_left(node) && !min_heap_node_get_right(node))
    {
        code[length] = '\0';
        code_table_set_code(table, min_heap_node_get_data(node), code);
        return;
    }

    if (min_heap_node_get_left(node))
    {
        code[length] = '0';
        huffman_tree_generate_code_for_node(min_heap_node_get_left(node), code, length + 1, table);
    }
    if (min_heap_node_get_right(node))
    {
        code[length] = '1';
        huffman_tree_generate_code_for_node(min_heap_node_get_right(node), code, length + 1, table);
    }
}

void huffman_tree_generate_codes(HuffmanTree* this, CodeTable* table)
{
    char code[MAX_SYMBOLS];
    huffman_tree_generate_code_for_node(huffman_tree_get_root(this), code, 0, table);
}