//
// Created by dominik on 1/3/24.
//

#include "red_black_tree_node.h"
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>


#define EMPTY 0

/**
 * @brief konśtruktor pre uzol, node Červeno-Čierneho stromu.
 *
 * @param this Ukazovateľ na uzol, ktorý má byť inicializovaný.
 */
void red_black_tree_node_init(RBTreeNode* this)
{
    this->value_size = EMPTY;
    this->value = NULL;
    this->nodeColour = black;
    this->code = EMPTY;
    this->parent = NULL;
    this->left = NULL;
    this->right = NULL;
}

/**
 * @brief Deštruktor pre uzol, node Červeno-Čierneho stromu.
 *
 * @param this Ukazovateľ na uzol, ktorý má byť zničený.
 */
void red_black_tree_node_destroy(RBTreeNode* this)
{
    if(this->value != NULL)
        free(this->value);

    this->value = NULL;
    this->value_size = EMPTY;
    this->nodeColour = black;
    this->code = EMPTY;
    this->parent = NULL;
    this->left = NULL;
    this->right = NULL;
}

/**
 * @brief Nastaví ľavého potomka pre daný uzol.
 *
 * @param this Ukazovateľ na uzol, pre ktorý sa má nastaviť ľavý potomok.
 * @param left Ukazovateľ na uzol, ktorý sa má nastaviť ako ľavý potomok.
 */
void red_black_tree_node_set_left_son(RBTreeNode* this, RBTreeNode* left)
{
    this->left = left;
}

/**
 * @brief Nastaví pravého potomka pre daný uzol.
 *
 * @param this Ukazovateľ na uzol, pre ktorý sa má nastaviť pravý potomok.
 * @param right Ukazovateľ na uzol, ktorý sa má nastaviť ako pravý potomok.
 */
void red_black_tree_node_set_right_son(RBTreeNode* this,RBTreeNode* right)
{
    this->right = right;
}

/**
 * @brief Nastaví rodiča pre daný uzol.
 *
 * @param this Ukazovateľ na uzol, pre ktorý sa má nastaviť rodič.
 * @param parent Ukazovateľ na uzol, ktorý sa má nastaviť ako rodič.
 */
void red_black_tree_node_set_parent(RBTreeNode* this,RBTreeNode* parent)
{
    this->parent = parent;
}

/**
 * @brief Získava ľavého potomka daného uzla.
 *
 * @param this Ukazovateľ na uzol, pre ktorý sa má získať ľavý potomok.
 * @return RBTreeNode* Ukazovateľ na ľavého potomka uzla.
 */
RBTreeNode* red_black_tree_node_get_left_son(RBTreeNode* this)
{
    return this->left;
}

/**
 * @brief Získava pravého potomka daného uzla.
 *
 * @param this Ukazovateľ na uzol, pre ktorý sa má získať pravý potomok.
 * @return RBTreeNode* Ukazovateľ na pravého potomka uzla.
 */
RBTreeNode* red_black_tree_node_get_right_son(RBTreeNode* this)
{
    return this->right;
}

/**
 * @brief Získava rodiča daného uzla.
 *
 * @param this Ukazovateľ na uzol, pre ktorý sa má získať rodič.
 * @return RBTreeNode* Ukazovateľ na rodiča uzla.
 */
RBTreeNode* red_black_tree_node_get_parent(RBTreeNode* this)
{
    return this->parent;
}

/**
 * @brief Získava strýka (brata rodiča) daného uzla.
 *
 * Ak rodič alebo rodičov rodič daného uzla neexistuje, vráti NULL.
 *
 * @param this Ukazovateľ na uzol, pre ktorý sa má získať strýko.
 * @return RBTreeNode* Ukazovateľ na strýka uzla, alebo NULL, ak strýko neexistuje.
 */
RBTreeNode* red_black_tree_get_uncle(RBTreeNode* this)
{
    if(this->parent == NULL)
        return NULL;

    if(this->parent->parent == NULL)
        return NULL;

    return ((this->parent->parent->right == this)?(this->parent->parent->left):(this->parent->parent->right));
}

/**
 * @brief Mení farbu daného uzla.
 *
 * Ak je farba uzla červená, zmení ju na čiernu, a naopak.
 *
 * @param this Ukazovateľ na uzol, ktorého farba má byť zmenená.
 */
void red_black_tree_node_recolour(RBTreeNode* this)
{
    this->nodeColour = (this->nodeColour == red) ? black : red;
}

/**
 * @brief Nastavuje farbu daného uzla.
 *
 * @param this Ukazovateľ na uzol, ktorého farba má byť nastavená.
 * @param nodeColour Nová farba uzla.
 */
void red_black_tree_node_set_colour(RBTreeNode* this, Colour nodeColour)
{
    this->nodeColour = nodeColour;
}

/**
 * @brief Nastavuje kód daného uzla.
 *
 * @param this Ukazovateľ na uzol, ktorému sa má nastaviť kód.
 * @param code Kód uzla.
 */
void red_black_tree_node_set_code(RBTreeNode* this, int code)
{
    this->code = code;
}

/**
 * @brief Nastavuje hodnotu daného uzla.
 *
 * @param this Ukazovateľ na uzol, ktorému sa má nastaviť hodnota.
 * @param value Ukazovateľ na pole bajtov reprezentujúce hodnotu.
 * @param value_size Veľkosť poľa bajtov.
 */
void red_black_tree_node_set_value(RBTreeNode* this, const unsigned char* value, size_t value_size)
{
    if(this->value != NULL)
        free(this->value);

    this->value = calloc(value_size,sizeof(unsigned char));
    for(size_t i = 0; i < value_size; i++)
    {
        this->value[i] = value[i];
    }
    this->value_size = value_size;
}

/**
 * @brief Získava farbu daného uzla.
 *
 * @param this Ukazovateľ na uzol, ktorého farba má byť zistená.
 * @return Colour Farba uzla.
 */
Colour red_black_tree_node_get_colour(RBTreeNode* this)
{
    return this->nodeColour;
}

/**
 * @brief Získava kód daného uzla.
 *
 * @param this Ukazovateľ na uzol, ktorého kód má byť zistený.
 * @return int Kód uzla.
 */
int red_black_tree_node_get_code(RBTreeNode* this)
{
    return this->code;
}

/**
 * @brief Získava hodnotu daného uzla.
 *
 * @param this Ukazovateľ na uzol, ktorého hodnota má byť zistená.
 * @return unsigned char* Ukazovateľ na hodnotu uzla.
 */
unsigned char* red_black_tree_node_get_value(RBTreeNode* this)
{
    return this->value;
}

/**
 * @brief Získava veľkosť hodnoty daného uzla.
 *
 * @param this Ukazovateľ na uzol, ktorého veľkosť hodnoty má byť zistená.
 * @return size_t Veľkosť hodnoty uzla.
 */
size_t red_black_tree_node_get_value_size(RBTreeNode* this)
{
    return this->value_size;
}

/**
 * @brief Serializuje uzol Červeno-Čierneho stromu do poľa bajtov.
 *
 * Táto funkcia konvertuje kód uzla a jeho hodnotu do sériového formátu bajtov.
 *
 * @param this Ukazovateľ na uzol, ktorý má byť serializovaný.
 * @param size Ukazovateľ na premennú, kde sa uloží veľkosť výsledného bajtového poľa.
 * @return unsigned char* Ukazovateľ na výsledné pole bajtov reprezentujúce uzol.
 */
unsigned char *red_black_tree_node_serialize(RBTreeNode *this, size_t *size)
{
    uint32_t code = htonl(this->code);
    uint32_t value_size = htonl((uint32_t)this->value_size);
    size_t buffer_size = sizeof(code) + sizeof(value_size) + this->value_size;

    unsigned char *buffer = malloc(buffer_size);

    memcpy(buffer, &code, sizeof(code));
    memcpy(buffer + sizeof(code), &value_size, sizeof(value_size));
    memcpy(buffer + sizeof(code) + sizeof(value_size), this->value, this->value_size);

    *size = buffer_size;
    return buffer;
}

/**
 * @brief Deserializuje uzol Červeno-Čierneho stromu z poľa bajtov.
 *
 * Táto funkcia konvertuje pole bajtov späť na kód a hodnotu uzla.
 *
 * @param buffer Ukazovateľ na pole bajtov, ktoré obsahuje serializovaný uzol.
 * @param code Ukazovateľ na premennú, kam sa uloží deserializovaný kód uzla.
 * @param value_size Ukazovateľ na premennú, kam sa uloží veľkosť hodnoty uzla.
 * @param value Ukazovateľ na premennú, kam sa uloží deserializovaná hodnota uzla.
 * @return unsigned char* Ukazovateľ na pozíciu v buffri nasledujúcu po deserializovanom uzle.
 */
unsigned char *red_black_tree_node_deserialize(unsigned char *buffer, int *code, size_t *value_size, unsigned char** value)
{
    uint32_t code_l;
    uint32_t value_size_l;

    memcpy(&code_l, buffer, sizeof(code_l));
    *code = ntohl(code_l);

    memcpy(&value_size_l, buffer + sizeof(code_l), sizeof(value_size_l));
    *value_size = ntohl(value_size_l);

    unsigned char *value_l = malloc(*value_size);

    memcpy(value_l, buffer + sizeof(code_l) + sizeof(value_size_l), *value_size);
    *value = value_l;

    return (buffer + sizeof(code_l) + sizeof(value_size_l) + *value_size - 1);
}
