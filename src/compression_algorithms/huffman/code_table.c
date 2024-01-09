//
// Created by dominik on 1/1/24.
//

#include "code_table.h"
#include "compression_algorithms/huffman/code_table_node.h"
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOLS 256

/**
 * @brief Konštruktor pre tabuľku kódov.
 *
 * @param this Ukazovateľ na inštanciu CodeTable, ktorá má byť inicializovaná.
 */
void code_table_init(CodeTable* this)
{
    this->array = calloc(MAX_SYMBOLS, sizeof(CodeNode));
    for (int i = 0; i < MAX_SYMBOLS; i++)
    {
        code_node_init((CodeNode*)((unsigned char*)this->array + i*sizeof(CodeNode)), i);
    }
}

/**
 * @brief Nastaví kód pre daný prvok v tabuľke.
 *
 * @param this Ukazovateľ na inštanciu CodeTable.
 * @param data Prvok, pre ktorý sa má nastaviť kód.
 * @param code Kód, ktorý sa má priradiť.
 * @param code_size Dĺžka kódu.
 */
void code_table_set_code(CodeTable* this, unsigned char data, const char* code, short code_size)
{
    code_node_set_code((CodeNode*)((unsigned char*)this->array + data*sizeof(CodeNode)), code, code_size);
}

/**
 * @brief Vráti uzol kódovej tabuľky pre daný prvok.
 *
 * @param this Ukazovateľ na inštanciu CodeTable.
 * @param data Prvok, pre ktorý sa má vrátiť uzol.
 * @return Ukazovateľ na CodeNode prislúchajúci danému prvku.
 */
CodeNode* code_table_get_node(CodeTable* this, unsigned char data)
{
    return (CodeNode*)((unsigned char*)this->array+ data*sizeof(CodeNode));
}

/**
 * @brief Deštruktor pre tabuľku kódov.
 *
 * @param this Ukazovateľ na inštanciu CodeTable.
 */
void code_table_destroy(CodeTable* this)
{
    for (int i = 0; i < MAX_SYMBOLS; i++)
    {
        code_node_destroy((CodeNode*)((unsigned char*)this->array + i*sizeof(CodeNode)));
    }
    free(this->array);
}

/**
 * @brief Vráti veľkosť kódovej tabuľky.
 *
 * @param this Ukazovateľ na inštanciu CodeTable.
 * @return Veľkosť tabuľky.
 */
int code_table_get_size(CodeTable* this)
{
    return MAX_SYMBOLS;
}

/**
 * @brief Serializuje kódovú tabuľku do poľa bajtov.
 *
 * @param this Ukazovateľ na inštanciu CodeTable.
 * @param size Ukazovateľ na premennú, kde sa uloží veľkosť výsledku.
 * @param valid_bits_in_last_byte Počet platných bitov v poslednom bajte.
 * @return Ukazovateľ na serializované dáta.
 */
unsigned char *code_table_serialize(CodeTable *this, size_t *size, int valid_bits_in_last_byte)
{
    // Resulting serialized table
    size_t table_serialized_size = 0;
    size_t table_serialized_capacity = 100;
    unsigned char *table_serialized = malloc(table_serialized_capacity);

    // For each serialized node
    CodeNode *node = NULL;
    unsigned char *node_serialized = NULL;
    size_t node_serialized_size = 0;

    for (int i = 0; i < MAX_SYMBOLS; i++)
    {
        node = (CodeNode *)((unsigned char *)this->array + i * sizeof(CodeNode));
        node_serialized = code_node_serialize(node, &node_serialized_size);

        if (table_serialized_size + node_serialized_size > table_serialized_capacity)
        {
            table_serialized_capacity *= 2;
            table_serialized = realloc(table_serialized, table_serialized_capacity);
        }
        memcpy(table_serialized + table_serialized_size, node_serialized, node_serialized_size); 
        table_serialized_size += node_serialized_size;
        node_serialized_size = 0;

        free(node_serialized);
    }

    if (table_serialized_size + 4 > table_serialized_capacity) {
            table_serialized_capacity += 4;
            table_serialized = realloc(table_serialized, table_serialized_capacity);
    }
    uint32_t len = htonl(valid_bits_in_last_byte);
    memcpy(table_serialized + table_serialized_size, &len, sizeof(len));
    table_serialized_size += sizeof(len);


    *size = table_serialized_size;
    return table_serialized;

}

/**
 * @brief Deserializuje tabuľku kódov zo zadaného bufferu.
 *
 * @param this Ukazovateľ na inštanciu CodeTable, ktorá má byť deserializovaná.
 * @param buffer Ukazovateľ na buffer obsahujúci serializované dáta.
 * @param size Veľkosť bufferu v bajtoch.
 * @param valid_bits_in_last_byte Ukazovateľ na premennú, do ktorej sa uloží počet platných bitov v poslednom bajte deserializovaných dát.
 */
void code_table_deserialize(CodeTable *this, unsigned char *buffer, size_t size, int *valid_bits_in_last_byte)
{
    this->array = calloc(MAX_SYMBOLS, sizeof(CodeNode));
    unsigned char* cur = buffer;
    CodeNode *node = NULL;
    for (int i = 0; i < MAX_SYMBOLS; i++)
    {
        node = (CodeNode *)((unsigned char *)this->array + i * sizeof(CodeNode));
        cur = code_node_deserialize(node, cur, 0);
        cur++;
    }
    int32_t len = 0;
    memcpy(&len, cur, sizeof(len));
    *valid_bits_in_last_byte = ntohl(len);
}
