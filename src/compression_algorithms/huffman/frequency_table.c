//
// Created by dominik on 12/31/23.
//

#include "frequency_table.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_SYMBOLS 256
#define EMPTY 0

/**
 * Konštruktor pre frekvenćnú tabuľku.
 *
 * @param this Ukazovateľ na inštanciu FrTable (frekvenčná tabuľka), ktorá má byť inicializovaná.
 */
void frequency_table_init(FrTable* this)
{
    this->array = calloc(MAX_SYMBOLS,sizeof(long long));
}

/**
 * Deštruktor pre frekvenčnú tabuľku.
 *
 * @param this Ukazovateľ na inštanciu FrTable (frekvenčná tabuľka), ktorá má byť zničená.
 */
void frequency_table_destroy(FrTable* this)
{
    free(this->array);
    this->array = NULL;
}

/**
 * Vytvorí frekvenčnú tabuľku na základe vstupných dát.
 *
 * @param this Ukazovateľ na inštanciu FrTable (frekvenčná tabuľka).
 * @param input Ukazovateľ na vstupné dáta.
 * @param inputSize Veľkosť vstupných dát.
 * @return Vracia true, ak bola tabuľka úspešne vytvorená.
 */
bool frequency_table_create(FrTable* this, unsigned char* input, size_t inputSize)
{
    for(size_t i = 0; i < inputSize; i++)
        this->array[input[i]]++;
    return true;
}

/**
 * Vráti frekvenciu pre daný index v frekvenčnú tabuľku.
 *
 * @param this Ukazovateľ na inštanciu FrTable (frekvenčná tabuľka).
 * @param index Index, pre ktorý sa má získať frekvencia.
 * @return Frekvencia na zadanom indexe.
 */
long long frequency_table_get_frequency(FrTable* this, size_t index)
{
    return this->array[index];
}
