//
// Created by dominik on 1/4/24.
//

#include "LZW_core.h"
#include "structures/tree/red_black_tree_node.h"

#define MAX_SYMBOLS (4*1024*1024)
#define INITIAL_OUTPUT_SIZE 8
#define OUT_CAPACITY 1024
#define SINGLE_CHAR 1
#define EMPTY 0

/**
 * @brief Kompresia vstupného reťazca pomocou LZW algoritmu.
 *
 * Táto funkcia komprimuje vstupný reťazec (input) pomocou LZW algoritmu, používa slovník
 * (dictionary) na ukladanie a vyhľadávanie sekvencií. Výsledkom je pole kódov predstavujúcich komprimované dáta.
 *
 * @param input Ukazovateľ na vstupný reťazec bajtov na kompresiu.
 * @param dictionary Ukazovateľ na slovník používaný v kompresii.
 * @param outputSize Ukazovateľ na premennú, kde sa uloží veľkosť výstupného poľa.
 * @param input_size Veľkosť vstupného reťazca.
 * @return int* Ukazovateľ na pole komprimovaných kódov.
 */
int* lzw_compress_encode(const unsigned char* input,RBTree* dictionary, size_t* outputSize, size_t input_size)
{
    unsigned char current_sequence[MAX_SYMBOLS + 1] = {0};
    size_t sequence_length = EMPTY;
    int code = EMPTY ;
    size_t i = EMPTY ;
    int oldCode = EMPTY ;
    int capacity = INITIAL_OUTPUT_SIZE;
    int *output = calloc(capacity, sizeof(int));
    *outputSize = EMPTY ;

    for (size_t u = 0; u < input_size; u++)
    {
        current_sequence[0] = input[u];
        if (red_black_tree_find_node_by_value( current_sequence,dictionary->root, SINGLE_CHAR ) == NULL)
        {
            red_black_tree_insert(dictionary,  code, current_sequence, SINGLE_CHAR);
            code++;
        }
    }

    for (i = 0; i < input_size; i++)
    {
        current_sequence[sequence_length] = input[i];


        if (red_black_tree_find_node_by_value( current_sequence,dictionary->root, sequence_length + 1) == NULL)
        {
            red_black_tree_insert(dictionary,  code, current_sequence,sequence_length + 1);

            if (*outputSize == capacity)
            {
                capacity *= 2;
                output = realloc(output, capacity * sizeof(int));
            }

            code++;
            sequence_length = 0;
            i--;
            output[*outputSize] = oldCode;
            (*outputSize)++;
        }
        else
        {
            if (*outputSize == capacity)
            {
                capacity *= 2;
                output = realloc(output, capacity * sizeof(int));
            }
            oldCode = red_black_tree_node_get_code(red_black_tree_find_node_by_value( current_sequence,dictionary->root,sequence_length+1));

            sequence_length++;
        }
    }
    if (sequence_length > 0)
    {
        if (*outputSize == capacity)
        {
            capacity *= 2;
            output = realloc(output, capacity * sizeof(int));
        }

        if (red_black_tree_find_node_by_value(current_sequence, dictionary->root,sequence_length) == NULL)
        {
            red_black_tree_insert(dictionary, code, current_sequence,sequence_length);
            output[*outputSize] = code;
        }
        else
            output[*outputSize] = red_black_tree_node_get_code(red_black_tree_find_node_by_value(current_sequence, dictionary->root,sequence_length));
        (*outputSize)++;
    }
    return output;
}

/**
 * @brief Dekompresia LZW komprimovaného reťazca.
 *
 * Táto funkcia dekomprimuje pole komprimovaných kódov (compressed) pomocou LZW algoritmu
 * a slovníka (dictionary), rekonštruuje pôvodný reťazec dát.
 *
 * @param compressed Ukazovateľ na pole komprimovaných kódov.
 * @param compressed_size Veľkosť poľa komprimovaných kódov.
 * @param decompressed_size Ukazovateľ na premennú, kde sa uloží veľkosť dekomprimovaného reťazca.
 * @param dictionary Ukazovateľ na slovník používaný v dekompresii.
 * @return unsigned char* Ukazovateľ na dekomprimovaný reťazec bajtov.
 */
unsigned char* lzw_decompress_encode(const int* compressed, size_t compressed_size, size_t* decompressed_size, RBTree* dictionary)
{
    int capacity = OUT_CAPACITY;
    unsigned char* decompressed = calloc(capacity, sizeof(unsigned char));

    size_t decompressed_size_l = EMPTY ;
    int code = EMPTY ;
    unsigned char* entry;


    for (int i = 0; i < compressed_size; i++) {
        code = compressed[i];
        RBTreeNode *tmpValueNode = red_black_tree_find_by_code(dictionary, code);
        if (tmpValueNode == NULL) {
            fprintf(stderr, "Poskodeny subor!! Code: '%c'", code);
            return NULL;
        }
        entry = red_black_tree_node_get_value(tmpValueNode);

        if (decompressed_size_l + red_black_tree_node_get_value_size(tmpValueNode) >= capacity) {
            capacity *= 2;
            unsigned char *temp = realloc(decompressed, capacity);

            decompressed = temp;
        }

        size_t x = decompressed_size_l;
        for (size_t z = 0; z < red_black_tree_node_get_value_size(tmpValueNode);z++)
        {
            decompressed[x] = entry[z];
            x++;
        }

        decompressed_size_l += red_black_tree_node_get_value_size(tmpValueNode);//strlen(entry);
    }

    *decompressed_size = decompressed_size_l;
    return decompressed;
}
