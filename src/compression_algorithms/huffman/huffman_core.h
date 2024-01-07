//
// Created by dominik on 12/31/23.
//

#ifndef TRANSMITR_HUFFMAN_CORE_H
#define TRANSMITR_HUFFMAN_CORE_H

#include "code_table.h"
#include "frequency_table.h"
#include "huffman_tree.h"
#include "structures/tree/min_heap.h"

void huffman_encode_from_file(unsigned char* input, size_t inputSize, unsigned char* output,int* outputSize,int* validBitsInLastByte, CodeTable *codeTable);
void huffman_decode(const unsigned char* input, int inputSize, int validBitsInLastByte, CodeTable* codeTable, unsigned char* output, int* outputSize);

#endif // TRANSMITR_HUFFMAN_CORE_H
