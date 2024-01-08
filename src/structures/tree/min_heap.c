//
// Created by dominik on 12/29/23.
//

#include "min_heap.h"
#include <stdlib.h>
#include <string.h>

#define DEFAULT_CAPACITY 10
#define EMPTY 0

/**
 * Konštruktor min-heap.
 *
 * @param this Ukazovateľ na inštanciu MinHeap, ktorá má byť inicializovaná.
 * @param node_size Veľkosť každého uzla v heap.
 *
 * Táto funkcia inicializuje min-heap nastavením počiatočnej kapacity na
 * DEFAULT_CAPACITY, veľkosti na EMPTY a alokáciou pamäte pre pole v ktorom budú uložené jednotlivé node.
 */
void min_heap_init(MinHeap* this, size_t node_size)
{
    this->node_size = node_size;
    this->capacity = DEFAULT_CAPACITY;
    this->size = EMPTY;
    this->array = calloc(DEFAULT_CAPACITY,node_size);
}

/**
 * Deštruktor pre min-heap.
 *
 * @param this Ukazovateľ na inštanciu MinHeap, ktorá má byť zničená.
 * @param destroy_node Ukazovateľ na funkciu, ktorá zničí jeden uzol haldy.
 *
 * Táto funkcia zničí min-heap uvoľnením pamäte priradenej pre každý node,uzol
 * a nakoniec uvoľní pamäť priradenú pre pole heap-u.
 */
void min_heap_destroy(MinHeap* this, void (*destroy_node)(void* node))
{
    for(int i = 0; i < this->size; i++)
    {
        destroy_node((unsigned char*)this->array + i*this->node_size);
    }

    free(this->array);
    this->capacity = EMPTY;
    this->size = EMPTY;
    this->node_size = EMPTY;
    this->array = NULL;
}

/**
 * Kontroluje, či má min-heap dostatočnú kapacitu.
 * Je to statická metóda.
 *
 * @param this Ukazovateľ na inštanciu MinHeap.
 * @return true, ak je kapacita dostatočná, inak false.
 */
static bool min_heap_has_capacity(MinHeap* this)
{
    return this->size != this->capacity;
}

/**
 * Zväčšuje kapacitu min-heap.
 * kapacita sa zväćšuje dvojnásobne oproti predchádzajúcej, nedostatočnej kapacite.
 *
 * @param this Ukazovateľ na inštanciu MinHeap.
 * @return true, ak sa kapacita úspešne zväčší, inak false.
 */
static bool min_heap_increase_capacity(MinHeap* this)
{
    this->capacity *= 2;
    void* tmp = realloc(this->array, this->capacity*this->node_size);

    if( tmp == NULL)
        return false;
    else
    {
        this->array = tmp;
        return true;
    }
}

/**
 * Vráti index rodičovského uzla v min-heap.
 *
 * @param index Index aktuálneho uzla.
 * @return Index rodičovského uzla.
 */
int min_heap_get_parent(int index)
{
    if(index <= 0)
        return -42;

    return (index - 1) / 2 ;
}

/**
 * Pridá nový uzol, node do min-heap.
 *
 * @param this Ukazovateľ na inštanciu MinHeap.
 * @param data Dáta pre nový uzol.
 * @param freq Frekvencia pre nový uzol.
 * @param get_Value Funkcia na získanie hodnoty uzla.
 * @param init Funkcia na inicializáciu nového uzla.
 * @param destroy Funkcia na zničenie uzla.
 * @return true, ak sa uzol pridá úspešne, inak false.
 */
bool min_heap_add(MinHeap* this,unsigned char data, long long freq,long long (*get_Value)(void*),void (*init)(void*,unsigned char,long long), void (*destroy)(void*))
{
    void* new = malloc(this->node_size);

    if (new == NULL)
        return false;

    init(new, data, freq);

    if(!min_heap_has_capacity(this))
        if(!min_heap_increase_capacity(this))
            return false;

    if(this->size == EMPTY)
    {
        memcpy((unsigned char*)this->array,new,this->node_size);
        this->size++;
        free(new);
        return true;
    }

    size_t i = this->size;
    this->size++;

    while (i > 0) {
        size_t parentIndex = min_heap_get_parent(i);
        if (get_Value(new) >= get_Value((void*)((unsigned char*)this->array + parentIndex * this->node_size))) {
            break;
        }

        memcpy((unsigned char*)this->array + i * this->node_size,(unsigned char*)this->array + parentIndex * this->node_size,this->node_size);
        i = parentIndex;
    }

    memcpy((unsigned char*)this->array + i * this->node_size, new, this->node_size);

    destroy(new);
    free(new);
    return true;
}

/**
 * Vráti aktuálnu veľkosť min-heap.
 *
 * @param this Ukazovateľ na inštanciu MinHeap.
 * @return Veľkosť min-heap.
 */
size_t min_heap_get_size(MinHeap* this)
{
    return this->size;
}

/**
 * Vráti koreňový uzol, node min-heap.
 *
 * @param this Ukazovateľ na inštanciu MinHeap.
 * @return Ukazovateľ na koreňový uzol, alebo NULL, ak je heap prázdny.
 */
void* min_heap_get_root(MinHeap* this)
{
    if (this->size == 0)
        return NULL;

    return this->array;
}

/**
 * Vráti index pravého dieťaťa uzla, node v min-heap.
 *
 * @param index Index rodičovského uzla.
 * @return Index pravého dieťaťa.
 */
int min_heap_get_right_child(int index)
{
    return (2*index + 2);
}

/**
 * Vráti index ľavého dieťaťa uzla, node v min-heap.
 *
 * @param index Index rodičovského uzla.
 * @return Index ľavého dieťaťa.
 */
int min_heap_get_left_child(int index)
{
    return (2*index + 1);
}

/**
 * Odstráni a vráti minimálny uzol, node z min-heap.
 *
 * @param this Ukazovateľ na inštanciu MinHeap.
 * @param get_Value Funkcia na získanie hodnoty uzla.
 * @param output Ukazovateľ na miesto, kam sa uloží odstránený uzol.
 */
void min_heap_remove_min(MinHeap* this,long long (*get_Value)(void*), void* output)
{
    if(this->size == EMPTY)
    {
        fprintf(stderr,"Empty heap!!");
        return;
    }

    if(this->size == 1)
    {
        this->size--;
        memcpy(output,this->array,this->node_size);
        return;
    }

    memcpy(output,this->array,this->node_size);
    this->size--;
    memcpy(this->array,(unsigned char*)this->array + this->size*this->node_size,this->node_size);



    int start_index = 0;
    while (start_index < this->size)
    {
        int min_index = start_index;
        int left = min_heap_get_left_child(start_index);
        int right = min_heap_get_right_child(start_index);

        if (left < this->size && get_Value((unsigned char*)this->array + left*this->node_size) < get_Value((unsigned char*)this->array + min_index*this->node_size))
            min_index = left;

        if (right < this->size && get_Value((unsigned char*)this->array + right*this->node_size) < get_Value((unsigned char*)this->array + min_index*this->node_size))
            min_index = right;

        if (min_index == start_index)
            break;

        void* tmp = malloc(this->node_size);
        memcpy(tmp,this->array + start_index*this->node_size,this->node_size);
        memcpy(this->array + start_index*this->node_size,this->array + min_index*this->node_size,this->node_size);
        memcpy(this->array + min_index*this->node_size,tmp,this->node_size);

        free(tmp);
        start_index = min_index;
    }
}

/**
 * Pridá uzol, node do min-heap.
 *
 * @param this Ukazovateľ na inštanciu MinHeap.
 * @param node Ukazovateľ na uzol, ktorý sa má pridať.
 * @param get_Value Funkcia na získanie hodnoty uzla.
 * @return true, ak sa uzol pridá úspešne, inak false.
 */
bool min_heap_add_node(MinHeap* this, void* node,long long (*get_Value)(void*))
{
    if (this == NULL || node == NULL)
        return false;

    if (!min_heap_has_capacity(this))
        if (!min_heap_increase_capacity(this))
            return false;

    size_t i = this->size;
    this->size++;
    while (i > 0)
    {
        size_t parentIndex = min_heap_get_parent(i);
        if (get_Value(node) >= get_Value((void*)((unsigned char*)this->array + parentIndex * this->node_size)))
            break;

        memcpy((unsigned char*)this->array + i * this->node_size,(unsigned char*)this->array + parentIndex * this->node_size,this->node_size);
        i = parentIndex;
    }

    memcpy((unsigned char*)this->array + i * this->node_size, node, this->node_size);
    return true;
}

/**
 * Vráti veľkosť uzla, node v min-heap.
 *
 * @param this Ukazovateľ na inštanciu MinHeap.
 * @return Veľkosť uzla.
 */
size_t min_heap_get_node_size(MinHeap* this)
{
    return this->node_size;
}

/**
 * Vráti kapacitu min-heap.
 *
 * @param this Ukazovateľ na inštanciu MinHeap.
 * @return Kapacita min-heap.
 */
size_t min_heap_get_capacity(MinHeap* this)
{
    return this->capacity;
}

/**
 * Vráti ukazovateľ na pole uzlov, node v min-heap.
 *
 * @param this Ukazovateľ na inštanciu MinHeap.
 * @return Ukazovateľ na pole uzlov.
 */
void* min_heap_get_array(MinHeap* this)
{
    return this->array;
}

/**
 * Nastaví pole uzlov, node min-heap.
 *
 * @param this Ukazovateľ na inštanciu MinHeap.
 * @param array Ukazovateľ na nové pole uzlov.
 */
void min_heap_set_array(MinHeap* this, void* array)
{
    this->array = array;
}

/**
 * Nastaví veľkosť min-heap.
 *
 * @param this Ukazovateľ na inštanciu MinHeap.
 * @param size Nová veľkosť haldy.
 */
void min_heap_set_size(MinHeap* this, size_t size)
{
    this->size = size;
}

/**
 * Nastaví veľkosť uzla, node v min-heap.
 *
 * @param this Ukazovateľ na inštanciu MinHeap.
 * @param node_size Nová veľkosť uzla.
 */
void min_heap_set_node_size(MinHeap* this, size_t node_size)
{
    this->node_size = node_size;
}

/**
 * Nastaví kapacitu min-heap.
 *
 * @param this Ukazovateľ na inštanciu MinHeap.
 * @param capacity Nová kapacita haldy.
 */
void min_heap_set_capacity(MinHeap* this, size_t capacity)
{
    this->capacity = capacity;
}