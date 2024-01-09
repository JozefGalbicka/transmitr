//
// Created by dominik on 12/31/23.
//
#include "huffman_tree.h"
#include <string.h>
#define MAX_SYMBOLS 256


/**
 * @brief Obalovacia funkcia pre získanie frekvencie uzla MinHeap.
 *
 * @param node Ukazovateľ na všeobecný (void) uzol, ktorý sa má pretypovať na MinHeapNode.
 * @return long long Frekvencia uzla MinHeap.
 */
static long long wrapper_min_heap_node_get_freq(void* node)
{
    return min_heap_node_get_freq((MinHeapNode*) node);
}

/**
 * @brief Obalovacia funkcia pre zničenie uzla MinHeap.
 *
 * @param node Ukazovateľ na všeobecný (void) uzol, ktorý sa má pretypovať na MinHeapNode a zničiť.
 */
static void wrapper_min_heap_node_destroy(void* node)
{
    return min_heap_node_destroy((MinHeapNode*) node);
}

/**
 * @brief Konštruktor pre Huffmanov strom s použitím MinHeap.
 *
 * @param this Ukazovateľ na Huffmanov strom, ktorý má byť inicializovaný.
 * @param minHeap Ukazovateľ na MinHeap, ktorý sa má použiť pre inicializáciu.
 */
void huffman_tree_init(HuffmanTree* this, MinHeap* minHeap )
{
    this->minHeap = malloc(sizeof(MinHeap));
    min_heap_set_array(this->minHeap, malloc(minHeap->node_size * minHeap->capacity));
    memcpy(min_heap_get_array(this->minHeap), min_heap_get_array(minHeap), min_heap_get_node_size(minHeap) * min_heap_get_size(minHeap));
    min_heap_set_node_size(this->minHeap,min_heap_get_node_size(minHeap));
    min_heap_set_capacity(this->minHeap,min_heap_get_capacity(minHeap));
    min_heap_set_size(this->minHeap,min_heap_get_size(minHeap));
}

/**
 * @brief deštruktor pre Huffmanov strom.
 *
 * @param this Ukazovateľ na Huffmanov strom, ktorý má byť zničený.
 */
void huffman_tree_destroy(HuffmanTree* this)
{
    huffman_tree_free(min_heap_get_root(this->minHeap));
    free(this->minHeap);
    this->minHeap = NULL;
}

/**
 * @brief Vytvára Huffmanov strom z MinHeap.
 *
 * @param this Ukazovateľ na Huffmanov strom, ktorý má byť vytvorený.
 */
void huffman_tree_create(HuffmanTree* this)
{

    if (min_heap_get_size(this->minHeap) == 1)
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

    while (min_heap_get_size(this->minHeap) > 1) {
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

/**
 * @brief Získava koreňový uzol Huffmanovho stromu.
 *
 * @param this Ukazovateľ na Huffmanov strom.
 * @return MinHeapNode* Ukazovateľ na koreňový uzol stromu.
 */
MinHeapNode* huffman_tree_get_root(HuffmanTree* this)
{
    return min_heap_get_root(this->minHeap);
}

/**
 * @brief Získava veľkosť (počet uzlov) Huffmanovho stromu.
 *
 * @param this Ukazovateľ na Huffmanov strom.
 * @return size_t Počet uzlov v strome.
 */
size_t huffman_tree_size(HuffmanTree* this)
{
    return min_heap_get_size(this->minHeap);
}

/**
 * @brief Rekurzívne uvoľní uzly Huffmanovho stromu.
 *
 * @param node Ukazovateľ na uzol, od ktorého sa má začať uvoľňovanie.
 */
void huffman_tree_free(MinHeapNode* node)
{
    if (node == NULL)
        return;

    huffman_tree_free(min_heap_node_get_left(node));
    huffman_tree_free(min_heap_node_get_right(node));
    free(node);
}

/**
 * @brief Generuje kódy pre konkrétny uzol v Huffmanovom strome.
 *
 * @param node Ukazovateľ na uzol, pre ktorý sa majú generovať kódy.
 * @param code Ukazovateľ na reťazec, kam sa ukladajú generované kódy.
 * @param length Dĺžka aktuálneho kódu.
 * @param table Ukazovateľ na tabuľku kódov, kam sa majú kódy ukladať.
 */
void huffman_tree_generate_code_for_node(MinHeapNode* node, char* code, short length, CodeTable* table)
{
    if (node == NULL)
        return;

    if (!min_heap_node_get_left(node) && !min_heap_node_get_right(node))
    {
        code_table_set_code(table, min_heap_node_get_data(node), code, length);
        return;
    }

    if (min_heap_node_get_left(node))
    {
        code[length] = '0';
        huffman_tree_generate_code_for_node(min_heap_node_get_left(node), code, length + 1 , table);
    }
    if (min_heap_node_get_right(node))
    {
        code[length] = '1';
        huffman_tree_generate_code_for_node(min_heap_node_get_right(node), code, length + 1, table);
    }
}

/**
 * @brief Generuje kódy pre všetky uzly v Huffmanovom strome.
 *
 * @param this Ukazovateľ na Huffmanov strom.
 * @param table Ukazovateľ na tabuľku kódov, kam sa majú kódy ukladať.
 */
void huffman_tree_generate_codes(HuffmanTree* this, CodeTable* table)
{
    char code[MAX_SYMBOLS];
    huffman_tree_generate_code_for_node(huffman_tree_get_root(this), code, 0, table);
}