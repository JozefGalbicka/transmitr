//
// Created by dominik on 1/4/24.
//

#ifndef TRANSMITR_LZW_CORE_H
#define TRANSMITR_LZW_CORE_H

#include "structures/tree/red_black_tree.h"
#include <stdio.h>
#include <string.h>

int* lzw_compress_encode(const unsigned char* input,RBTree* dictionary, size_t* outputSize,size_t input_size);
unsigned char* lzw_decompress_encode(const int* compressed, size_t compressed_size, RBTree* dictionary);

#endif // TRANSMITR_LZW_CORE_H
