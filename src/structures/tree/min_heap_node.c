//
// Created by dominik on 12/30/23.
//
#define EMPTY 0
#include "min_heap_node.h"

/**
 * Konstruktor uzlu,node min-heap..
 *
 * @param this Ukazovateľ na MinHeapNode, ktorý sa má inicializovať.
 * @param data Dáta, ktoré sa majú uložiť do uzla.
 * @param freq Frekvencia, ktorá sa má priradiť uzlu.
 */
void min_heap_node_init(MinHeapNode* this, unsigned char data, long long freq)
{
    this->data = data;
    this->freq = freq;
    this->right = NULL;
    this->left = NULL;
}

/**
 * Vráti frekvenciu uzla,node min-heap.
 *
 * @param this Ukazovateľ na MinHeapNode.
 * @return Frekvencia uzla.
 */
long long min_heap_node_get_freq(MinHeapNode* this)
{
    return this->freq;
}

/**
 * Vráti dáta uzla,nado min-heap.
 *
 * @param this Ukazovateľ na MinHeapNode.
 * @return Dáta uzla.
 */
unsigned char min_heap_node_get_data(MinHeapNode* this)
{
    return this->data;
}

/**
 * Nastaví frekvenciu uzla, node min-heap.
 *
 * @param this Ukazovateľ na MinHeapNode.
 * @param freq Nová frekvencia, ktorá sa má nastaviť.
 */
void min_heap_node_set_freq(MinHeapNode* this, long long freq)
{
    this->freq = freq;
}

/**
 * Nastaví dáta uzla, node min-heap.
 *
 * @param this Ukazovateľ na MinHeapNode.
 * @param data Nové dáta, ktoré sa majú nastaviť.
 */
void min_heap_node_set_data(MinHeapNode* this, unsigned char data)
{
    this->data = data;
}

/**
 * Deštruktor uzolu ,node min-heap.
 *
 * @param this Ukazovateľ na MinHeapNode.
 */
void min_heap_node_destroy(MinHeapNode* this)
{
    this->freq = EMPTY;
    this->data = EMPTY;
}

/**
 * Nastaví pravé dieťa pre uzol,node min-heap.
 *
 * @param this Ukazovateľ na MinHeapNode.
 * @param right Ukazovateľ na pravé dieťa, ktoré sa má nastaviť.
 */
void min_heap_node_set_right(MinHeapNode* this, MinHeapNode* right)
{
    this->right = right;
}

/**
 * Nastaví ľavé dieťa pre uzol, node min-heap.
 *
 * @param this Ukazovateľ na MinHeapNode.
 * @param left Ukazovateľ na ľavé dieťa, ktoré sa má nastaviť.
 */
void min_heap_node_set_left(MinHeapNode* this, MinHeapNode* left)
{
    this->left = left;
}

/**
 * Vráti pravé dieťa uzla, node min-heap.
 *
 * @param this Ukazovateľ na MinHeapNode.
 * @return Ukazovateľ na pravé dieťa uzla.
 */
MinHeapNode* min_heap_node_get_right(MinHeapNode* this)
{
    return this->right;
}

/**
 * Vráti ľavé dieťa uzla, heap min-heap.
 *
 * @param this Ukazovateľ na MinHeapNode.
 * @return Ukazovateľ na ľavé dieťa uzla.
 */
MinHeapNode* min_heap_node_get_left(MinHeapNode* this)
{
    return this->left;
}

