//
// Created by dominik on 1/1/24.
//

#include "code_table_node.h"
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define EMPTY 0

/**
 * @brief Konštruktor pre prvok, node tabuky kódov.
 *
 * @param this Ukazovateľ na CodeNode, ktorý sa má inicializovať.
 * @param data Dáta, ktoré sa majú uložiť do uzla.
 */
void code_node_init(CodeNode* this, unsigned char data)
{
    this->code_size = EMPTY;
    this->data = data;
    this->code = NULL;
    code_node_set_code(this, "", EMPTY);
}

/**
 * @brief Nastaví kód a jeho veľkosť pre prvok, node tabuky kódov.
 *
 * @param this Ukazovateľ na CodeNode.
 * @param code Ukazovateľ na reťazec, ktorý predstavuje kód.
 * @param code_size Veľkosť kódu.
 */
void code_node_set_code(CodeNode* this, const char* code, short code_size) {
    if (code_size != 0)
    {
        free(this->code);
        this->code = realloc(this->code, sizeof(char) * code_size);
    }
    for(size_t i = 0; i < code_size;i++)
    {
        this->code[i] = code[i];
    }
    this->code_size = code_size;
}

/**
 * @brief Deštruktor pre prvok, node tabuky kódov.
 *
 * @param this Ukazovateľ na CodeNode, ktorý sa má zničiť.
 */
void code_node_destroy(CodeNode* this)
{
    free(this->code);
    this->code_size = EMPTY;
    this->code = NULL;
}

/**
 * @brief Vráti kód pre prvok, node tabuky kódov.
 *
 * @param this Ukazovateľ na CodeNode.
 * @return Ukazovateľ na reťazec predstavujúci kód prvku.
 */
char* code_node_get_code(CodeNode* this)
{
    return this->code;
}

/**
 * @brief Vráti veľkosť kódu pre prvok, node tabuky kódov.
 *
 * @param this Ukazovateľ na CodeNode.
 * @return Veľkosť kódu.
 */
short code_node_get_code_size(CodeNode* this)
{
    return this->code_size;
}

/**
 * @brief Serializuje prvok CodeNode do bajtovej postupnosti.
 *
 * Táto funkcia konvertuje dáta a kódovanú reprezentáciu uzla CodeNode do serializovanej formy.
 *
 * @param this Ukazovateľ na inštanciu CodeNode, ktorá má byť serializovaná.
 * @param size Ukazovateľ na premennú, do ktorej sa uloží veľkosť výsledného bufferu.
 * @return Ukazovateľ na dynamicky alokovaný buffer obsahujúci serializované dáta.
 */
unsigned char *code_node_serialize(CodeNode *this, size_t *size) 
{
    uint16_t len = htons(this->code_size);
    size_t buffer_size = sizeof(this->data) + sizeof(len) + this->code_size;
    unsigned char *buffer = malloc(buffer_size);

    memcpy(buffer, &len, sizeof(len));
    memcpy(buffer + sizeof(len), &this->data, sizeof(this->data));
    memcpy(buffer + sizeof(len) + sizeof(this->data), this->code, this->code_size);

    *size = buffer_size;

    return buffer;
}

/**
 * @brief Deserializuje prvok CodeNode z bajtovej postupnosti.
 * Táto funkcia rekonštruuje inštanciu CodeNode z jej serializovanej formy.
 *
 * @param this Ukazovateľ na inštanciu CodeNode, ktorá má byť deserializovaná.
 * @param buffer Ukazovateľ na buffer obsahujúci serializované dáta.
 * @param size Veľkosť bufferu v bajtoch.
 * @return Ukazovateľ na miesto v bufferi nasledujúce po spracovaných dátach.
 */
unsigned char *code_node_deserialize(CodeNode *this, unsigned char *buffer, size_t size)
{
    uint16_t len;
    memcpy(&len, buffer, sizeof(len));
    this->code_size = ntohs(len);
    memcpy(&this->data, buffer + sizeof(len), sizeof(this->data));
    this->code = malloc(this->code_size);
    memcpy(this->code, buffer + sizeof(len) + sizeof(this->data), this->code_size);

    return (buffer + sizeof(len) + sizeof(this->data) + this->code_size - 1);
}
