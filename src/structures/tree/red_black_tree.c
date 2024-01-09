//
// Created by dominik on 1/3/24.
//
#include "red_black_tree.h"
#include "structures/tree/red_black_tree_node.h"
#include <stdio.h>
#include <string.h>

#define EMPTY 0

/**
 * @brief Konštruktor pre Červeno-Čierny strom.
 *
 * Táto funkcia nastaví koreň na NULL a veľkosť na EMPTY.
 *
 * @param this Ukazovateľ na Červeno-Čierny strom, ktorý má byť inicializovaný.
 */
void red_black_tree_init(RBTree* this)
{
    this->root = NULL;
    this->size = EMPTY;
}

/**
 * @brief Statická funkcia ktorá vykonáva postorder prechod a zničí každý uzol.
 * @details Toto je interná pomocná funkcia.
 *
 * @param node Aktuálny uzol v prechode.
 * @param node_function Ukazovateľ na funkciu pre špecifické zničenie uzla.
 */
static void red_black_tree_postorder_destroy(RBTreeNode* node, void (*node_function)(RBTreeNode*))
{
    if (node != NULL)
    {
        red_black_tree_postorder_destroy(red_black_tree_node_get_left_son(node), node_function);
        red_black_tree_postorder_destroy(red_black_tree_node_get_right_son(node), node_function);
        node_function(node);
        free(node);
    }
}

/**
 * @brief Deśtukrtor pre Červeno-Čierny strom.
 *
 * @param this Ukazovateľ na Červeno-Čierny strom, ktorý má byť zničený.
 */
void red_black_tree_destroy(RBTree* this)
{
    red_black_tree_postorder_destroy(this->root, red_black_tree_node_destroy);
    this->root = NULL;
    this->size = EMPTY;
}

/**
 * @brief Vykonáva ľavú rotáciu okolo daného uzla.
 *
 * @param this Ukazovateľ na Červeno-Čierny strom.
 * @param node Ukazovateľ na uzol, okolo ktorého sa má vykonať rotácia.
 */
static void red_black_tree_left_rotation(RBTree* this, RBTreeNode* node)
{
    if(node == NULL || red_black_tree_node_get_right_son(node) == NULL)
        return;

    RBTreeNode* newTop = red_black_tree_node_get_right_son(node);
    red_black_tree_node_set_right_son(node,red_black_tree_node_get_left_son(newTop));

    if(red_black_tree_node_get_left_son(newTop) != NULL)
        red_black_tree_node_set_parent(red_black_tree_node_get_left_son(newTop), node);

    red_black_tree_node_set_parent(newTop, red_black_tree_node_get_parent(node));

    if(red_black_tree_node_get_parent(node) != NULL)
    {
        if(red_black_tree_node_get_left_son(red_black_tree_node_get_parent(node)) == node)
            red_black_tree_node_set_left_son(red_black_tree_node_get_parent(node), newTop);
        else
            red_black_tree_node_set_right_son(red_black_tree_node_get_parent(node), newTop);
    }
    else
        this->root = newTop;

    red_black_tree_node_set_left_son(newTop, node);
    red_black_tree_node_set_parent(node, newTop);
}

/**
 * @brief Vykonáva pravú rotáciu okolo daného uzla.
 *
 * @param this Ukazovateľ na Červeno-Čierny strom.
 * @param node Ukazovateľ na uzol, okolo ktorého sa má vykonať rotácia.
 */
static void red_black_tree_right_rotation(RBTree* this, RBTreeNode* node)
{
    if (node == NULL || red_black_tree_node_get_left_son(node) == NULL)
        return;

    RBTreeNode *newTop = red_black_tree_node_get_left_son(node);
    red_black_tree_node_set_left_son(node, red_black_tree_node_get_right_son(newTop));

    if (red_black_tree_node_get_right_son(newTop) != NULL)
        red_black_tree_node_set_parent(red_black_tree_node_get_right_son(newTop), node);

    red_black_tree_node_set_parent(newTop, red_black_tree_node_get_parent(node));

    if (red_black_tree_node_get_parent(node) != NULL) {
        if (red_black_tree_node_get_left_son(red_black_tree_node_get_parent(node)) == node)
            red_black_tree_node_set_left_son(red_black_tree_node_get_parent(node), newTop);
        else
            red_black_tree_node_set_right_son(red_black_tree_node_get_parent(node), newTop);
    } else
        this->root = newTop;

    red_black_tree_node_set_right_son(newTop, node);
    red_black_tree_node_set_parent(node, newTop);
}

/**
 * @brief Opravuje vlastnosti Červeno-Čierneho stromu.
 *
 * Táto funkcia zabezpečuje, že strom dodržiava všetky pravidlá Červeno-Čierneho stromu,
 * ako sú farby uzlov a vyváženosť stromu.
 *
 * @param this Ukazovateľ na Červeno-Čierny strom.
 * @param node Ukazovateľ na uzol, kde sa má začať oprava.
 */
static void red_black_tree_rules_repair(RBTree* this, RBTreeNode* node)
{
    RBTreeNode *parent = red_black_tree_node_get_parent(node);
    RBTreeNode *grandparent = NULL;
    if(parent != NULL)
        grandparent = red_black_tree_node_get_parent(parent);

    while (parent != NULL && red_black_tree_node_get_colour(parent) == red && grandparent != NULL)
    {
        if (parent == red_black_tree_node_get_left_son(grandparent))
        {
            RBTreeNode *uncle = red_black_tree_node_get_right_son(grandparent);

            if (uncle != NULL && red_black_tree_node_get_colour(uncle) == red)
            {
                red_black_tree_node_recolour(parent);
                red_black_tree_node_recolour(uncle);
                red_black_tree_node_recolour(grandparent);
                node = grandparent;
            }
            else
            {
                if (node == red_black_tree_node_get_right_son(parent))
                {
                    red_black_tree_left_rotation(this, parent);
                    node = parent;
                    parent = red_black_tree_node_get_parent(node);
                }

                red_black_tree_node_recolour(parent);
                red_black_tree_node_recolour(grandparent);
                red_black_tree_right_rotation(this, grandparent);
            }
        }
        else
        {
            RBTreeNode *uncle = red_black_tree_node_get_left_son(grandparent);

            if (uncle != NULL && red_black_tree_node_get_colour(uncle) == red)
            {
                red_black_tree_node_recolour(parent);
                red_black_tree_node_recolour(uncle);
                red_black_tree_node_recolour(grandparent);
                node = grandparent;
            }
            else
            {
                if (node == red_black_tree_node_get_left_son(parent))
                {
                    red_black_tree_right_rotation(this, parent);
                    node = parent;
                    parent = red_black_tree_node_get_parent(node);
                }

                red_black_tree_node_recolour(parent);
                red_black_tree_node_recolour(grandparent);
                red_black_tree_left_rotation(this, grandparent);
            }
        }
        parent = red_black_tree_node_get_parent(node);
        if(parent != NULL)
            grandparent = red_black_tree_node_get_parent(parent);
    }

    red_black_tree_node_set_colour(this->root, black);
}

/**
 * @brief Vloží nový uzol, node do Červeno-Čierneho stromu.
 *
 * @param this Ukazovateľ na Červeno-Čierny strom.
 * @param code Celé číslo reprezentujúce uzol.
 * @param value Ukazovateľ na hodnotu, ktorá má byť uložená v uzle.
 * @param value_size Veľkosť ukladanej hodnoty.
 */
void red_black_tree_insert(RBTree* this,int code,const unsigned char* value,size_t value_size)
{
    RBTreeNode* newNode = malloc(sizeof(RBTreeNode));
    red_black_tree_node_init(newNode);
    red_black_tree_node_set_code(newNode, code);
    red_black_tree_node_set_value(newNode, value, value_size);
    red_black_tree_node_set_colour(newNode, red);


    RBTreeNode* potFather = NULL;
    RBTreeNode* potSon = this->root;

    while (potSon != NULL)
    {
        potFather = potSon;
        if (red_black_tree_node_get_code(newNode) < red_black_tree_node_get_code(potSon))
            potSon = red_black_tree_node_get_left_son(potSon);
        else
            potSon = red_black_tree_node_get_right_son(potSon);
    }

    red_black_tree_node_set_parent(newNode, potFather);

    if (potFather == NULL)
        this->root = newNode;
    else if (red_black_tree_node_get_code(newNode) < red_black_tree_node_get_code(potFather))
        red_black_tree_node_set_left_son(potFather, newNode);
    else
        red_black_tree_node_set_right_son(potFather, newNode);

    red_black_tree_rules_repair(this, newNode);
    this->size++;
}

/**
 * @brief Statická funkcia ktorá získava uzol s najmenšou hodnotou v strome alebo v jeho podstrome.
 *
 * Táto funkcia prechádza stromom alebo podstromom od daného uzla vždy k najľavejšiemu potomkovi,
 * čím nájde uzol s najmenšou hodnotou.
 *
 * @param node Ukazovateľ na uzol, od ktorého sa má hľadať najmenší uzol.
 * @return RBTreeNode* Ukazovateľ na najmenší uzol.
 */
static RBTreeNode* red_black_tree_get_minimum_node(RBTreeNode* node)
{
    while (red_black_tree_node_get_left_son(node) != NULL)
    {
        node = red_black_tree_node_get_left_son(node);
    }
    return node;
}

/**
 * @brief Statická funkcia ktorá transplantuje jeden uzol za iný v strome.
 *
 * Táto funkcia nahrádza jedným stromovým uzlom (nodeA) iným uzlom (nodeB),
 * pričom zachováva stromovú štruktúru.
 *
 * @param this Ukazovateľ na Červeno-Čierny strom.
 * @param nodeA Ukazovateľ na uzol, ktorý má byť nahradený.
 * @param nodeB Ukazovateľ na uzol, ktorým sa nahradí nodeA.
 */
static void red_black_tree_node_transplant(RBTree* this, RBTreeNode* nodeA, RBTreeNode* nodeB) {
    if (red_black_tree_node_get_parent(nodeA) == NULL)
        this->root = nodeB;
    else if (nodeA == red_black_tree_node_get_left_son(red_black_tree_node_get_parent(nodeA)))
        red_black_tree_node_set_left_son(red_black_tree_node_get_parent(nodeA),nodeB);
    else
        red_black_tree_node_set_right_son(red_black_tree_node_get_parent(nodeA), nodeB);

    if (nodeB != NULL)
        red_black_tree_node_set_parent(nodeB, red_black_tree_node_get_parent(nodeA));
}

/**
 * @brief Odstráni uzol zo stromu.
 *
 * Táto funkcia odstráni špecifikovaný uzol zo stromu a potom vykoná príslušné úpravy na udržanie
 * vlastností Červeno-Čierneho stromu.
 *
 * @param this Ukazovateľ na Červeno-Čierny strom.
 * @param node Ukazovateľ na uzol, ktorý má byť odstránený.
 * @return RBTreeNode* Ukazovateľ na odstránený uzol.
 */
RBTreeNode* red_black_tree_remove(RBTree* this, RBTreeNode* node) {
    if (node == NULL)
        return NULL;

    RBTreeNode* y = node;
    RBTreeNode* x;
    Colour originalColor = red_black_tree_node_get_colour(y);

    if (red_black_tree_node_get_left_son(node) == NULL)
    {
        x = red_black_tree_node_get_right_son(node);
        red_black_tree_node_transplant(this, node, x);
    }
    else if (red_black_tree_node_get_right_son(node) == NULL)
    {
        x = red_black_tree_node_get_left_son(node);
        red_black_tree_node_transplant(this, node, x);
    }
    else
    {
        y = red_black_tree_get_minimum_node(red_black_tree_node_get_right_son(node));
        originalColor = red_black_tree_node_get_colour(y);
        x = red_black_tree_node_get_right_son(y);

        if (red_black_tree_node_get_parent(y) != node)
        {
            red_black_tree_node_transplant(this, y, x);
            red_black_tree_node_set_right_son(y, red_black_tree_node_get_right_son(node));

            if (red_black_tree_node_get_right_son(y) != NULL)
                red_black_tree_node_set_parent(red_black_tree_node_get_right_son(y), y);
        }

        red_black_tree_node_transplant(this, node, y);
        red_black_tree_node_set_left_son(y, red_black_tree_node_get_left_son(node));

        if (red_black_tree_node_get_left_son(y) != NULL)
            red_black_tree_node_set_parent(red_black_tree_node_get_left_son(y), y);

        red_black_tree_node_set_colour(y, originalColor);
    }

    if (originalColor == black)
        red_black_tree_rules_repair(this, x);

    this->size--;
    return node;
}

/**
 * @brief Hľadá uzol podľa jeho hodnoty.
 *
 * Táto funkcia prechádza stromom alebo podstromom a hľadá uzol, ktorého hodnota zodpovedá zadanému vstupu.
 *
 * @param value Ukazovateľ na hodnotu, podľa ktorej sa hľadá uzol.
 * @param subRoot Ukazovateľ na koreň podstromu, v ktorom sa má hľadať.
 * @param value_size Veľkosť hodnoty.
 * @return RBTreeNode* Ukazovateľ na nájdený uzol, alebo NULL, ak sa taký nenájde.
 */
RBTreeNode* red_black_tree_find_node_by_value(unsigned char* value, RBTreeNode* subRoot, size_t value_size)
{
    if (subRoot == NULL)
        return NULL;

    unsigned char *tmp = red_black_tree_node_get_value(subRoot);
    if (red_black_tree_node_get_value_size(subRoot) == value_size)
    {
        size_t i;
        for (i = 0; i < value_size; i++)
        {
            if (tmp[i] != value[i])
                break;
        }

        if(i == value_size)
        return subRoot;
    }


    RBTreeNode* foundNode = red_black_tree_find_node_by_value(value, red_black_tree_node_get_left_son(subRoot),value_size);

    if (foundNode != NULL)
        return foundNode;

    return red_black_tree_find_node_by_value(value, red_black_tree_node_get_right_son(subRoot),value_size);
}

/**
 * @brief Nájde uzol podľa jeho kódu.
 *
 * @param this Ukazovateľ na Červeno-Čierny strom.
 * @param code Kód uzla, ktorý sa má nájsť.
 * @return RBTreeNode* Ukazovateľ na nájdený uzol alebo NULL, ak sa nenájde.
 */
RBTreeNode* red_black_tree_find_by_code(RBTree* this, int code) {
    RBTreeNode* current = this->root;

    while (current != NULL)
    {
        if (red_black_tree_node_get_code(current) == code)
            return current;
        else if (code < red_black_tree_node_get_code(current))
            current = red_black_tree_node_get_left_son(current);
        else
            current = red_black_tree_node_get_right_son(current);
    }

    return NULL;
}

/**
 * @brief Získava veľkosť (počet uzlov) Červeno-Čierneho stromu.
 *
 * @param this Ukazovateľ na Červeno-Čierny strom, ktorého veľkosť má byť zistená.
 * @return size_t Počet uzlov v strome.
 */
size_t red_black_tree_get_size(RBTree* this)
{
    return this->size;
}

/**
 * @brief Získa všetky uzly stromu v postorder poradí.
 *
 * Táto funkcia prechádza stromom a ukladá uzly do zadaného poľa v postorder poradí,
 * t.j. najprv navštívi ľavého potomka, potom pravého potomka a nakoniec rodičovský uzol.
 *
 * @param node Ukazovateľ na aktuálny uzol v prechode.
 * @param nodeArray Ukazovateľ na pole uzlov, kam budú uzly ukladané.
 * @param size Ukazovateľ na premennú, kde sa ukladá počet uzlov.
 */
void red_black_tree_postorder_get_nodes(RBTreeNode* node,RBTreeNode** nodeArray, size_t* size)
{
    if (node != NULL)
    {
        red_black_tree_postorder_get_nodes(red_black_tree_node_get_left_son(node), nodeArray, size);
        red_black_tree_postorder_get_nodes(red_black_tree_node_get_right_son(node), nodeArray, size);
        nodeArray[*size] = node;
        *size = (*size) + 1;
    }
}

/**
 * @brief Serializuje Červeno-Čierny strom do poľa bajtov.
 *
 * Táto funkcia prechádza stromom a konvertuje jeho uzly na sériu bajtov.
 *
 * @param this Ukazovateľ na Červeno-Čierny strom, ktorý má byť serializovaný.
 * @param size Ukazovateľ na premennú, kde sa uloží veľkosť výsledného bajtového poľa.
 * @return unsigned char* Ukazovateľ na výsledné pole bajtov reprezentujúce strom.
 */
unsigned char *red_black_tree_serialize(RBTree *this, size_t *size)
{
    RBTreeNode *node_array[this->size];
    size_t node_array_size = 0;

    RBTreeNode *node;
    unsigned char *node_buf = NULL;
    size_t node_buf_size = 0;

    size_t buf_capacity = 100;
    size_t buf_size = 0;
    unsigned char *buf = malloc(buf_capacity);

    red_black_tree_postorder_get_nodes(this->root, node_array, &node_array_size);
    for (int i = 0; i < node_array_size; i++)
    {
        node = node_array[i];

        node_buf = red_black_tree_node_serialize(node, &node_buf_size);
        //printf("Code: '%d'\n", node->code);
        //printf("Value size: %zu\n", node->value_size);
        //printf("Value: %.*s\n", node->value_size, node->value);
        while (buf_size + node_buf_size > buf_capacity) {
            buf_capacity *= 2;
            buf = realloc(buf, buf_capacity);
        }
        memcpy(buf + buf_size, node_buf, node_buf_size);
        buf_size += node_buf_size;

        free(node_buf);
        node_buf = NULL;
        node_buf_size = 0;
    }
    if (node_array_size != this->size)
    {
        fprintf(stderr, "Post order did not include all nodes in serialization\n");
        exit(101);
    }

    *size = buf_size;
    return buf;
}

/**
 * @brief Deserializuje Červeno-Čierny strom z poľa bajtov.
 *
 * Táto funkcia berie pole bajtov a rekonštruuje z neho Červeno-Čierny strom.
 *
 * @param this Ukazovateľ na Červeno-Čierny strom, do ktorého sa má deserializovať.
 * @param buffer Ukazovateľ na pole bajtov, ktoré obsahuje serializovaný strom.
 * @param size Veľkosť bajtového poľa.
 */
void red_black_tree_deserialize(RBTree *this, unsigned char *buffer, size_t size)
{

    size_t curr_index = 0;
    unsigned char *curr_buf = buffer;
    unsigned char *new_curr_buf = NULL;

    int code = 0;
    size_t value_size;
    unsigned char *value = NULL;

    while (curr_index < size)
    {
        new_curr_buf = red_black_tree_node_deserialize(curr_buf, &code, &value_size, &value);
        //printf("Code: '%d'\n", code);
        //printf("Value size: %zu\n", value_size);
        //printf("Value: %.*s\n", value_size, value);
        red_black_tree_insert(this, code, value, value_size);
        free(value);

        new_curr_buf++;
        curr_index += (new_curr_buf - curr_buf);
        curr_buf = new_curr_buf;
        new_curr_buf = NULL;
    }
    //printf("%zu/%zu", curr_index, size);
}
