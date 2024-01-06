//
// Created by dominik on 12/31/23.
//

#include "frequency_table.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_SYMBOLS 256
#define EMPTY 0

void frequency_table_init(FrTable* this)
{
    this->array = calloc(MAX_SYMBOLS,sizeof(long long));
}


void frequency_table_destroy(FrTable* this)
{
    free(this->array);
    this->array = NULL;
}


bool frequency_table_create(FrTable* this, unsigned char* input, size_t inputSize)
{


    for(size_t i = 0; i < inputSize; i++)
        this->array[input[i]]++;
    return true;
}


long long frequency_table_get_frequency(FrTable* this, size_t index)
{
    return this->array[index];
}
