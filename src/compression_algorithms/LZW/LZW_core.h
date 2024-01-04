//
// Created by dominik on 1/4/24.
//

#ifndef TRANSMITR_LZW_CORE_H
#define TRANSMITR_LZW_CORE_H

#include "structures/tree/red_black_tree.h"
#include <stdio.h>
#include <string.h>

int* lzw_compress_encode(const unsigned char* input,RBTree* dictionary, int* outputSize);
unsigned char* lzw_decompress_encode(const int* compressed, int compressed_size, RBTree* dictionary);

#endif // TRANSMITR_LZW_CORE_H