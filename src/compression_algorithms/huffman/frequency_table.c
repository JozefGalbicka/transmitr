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


bool frequency_table_create(FrTable* this, const char *filename)
{
    FILE* file = fopen(filename, "rb");

    if (file == NULL)
        return false;

    int c;
    while ((c = fgetc(file)) != EOF)
    {
        this->array[c]++;
    }

    fclose(file);
    return true;
}
