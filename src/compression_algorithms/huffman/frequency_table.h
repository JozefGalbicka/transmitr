//
// Created by dominik on 12/31/23.
//

#ifndef TRANSMITR_FREQUENCY_TABLE_H
#define TRANSMITR_FREQUENCY_TABLE_H

#include <stdbool.h>

typedef struct FrequencyTable
{
    long long *array;
}FrTable;

void frequency_table_init(FrTable* this);
void frequency_table_destroy(FrTable* this);
bool frequency_table_create(FrTable* this, const char *filename);



#endif // TRANSMITR_FREQUENCY_TABLE_H
