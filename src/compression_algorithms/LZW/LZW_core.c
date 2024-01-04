//
// Created by dominik on 1/4/24.
//

#include "LZW_core.h"

#define MAX_SYMBOLS 256
#define INITIAL_OUTPUT_SIZE 8
#define OUT_CAPACITY 1024

int* lzw_compress_encode(const unsigned char* input,RBTree* dictionary, int* outputSize)
{
    unsigned char current_sequence[MAX_SYMBOLS + 1] = {0};
    size_t sequence_length = 0;
    int code = 0;
    size_t i = 0;

    int capacity = INITIAL_OUTPUT_SIZE;
    int* output = calloc(capacity, sizeof(int));
    *outputSize = 0;

    for (i = 0; input[i] != '\0'; i++)
    {
        current_sequence[sequence_length] = input[i];
        current_sequence[sequence_length + 1] = '\0';

        if (red_black_tree_find_node_by_value( current_sequence,dictionary->root) == NULL)
        {
            red_black_tree_insert(dictionary,  code, current_sequence);

            if (*outputSize == capacity)
            {
                capacity *= 2;
                output = realloc(output, capacity * sizeof(int));
            }

            output[*outputSize] = code;
            code++;
            (*outputSize)++;
            sequence_length = 0;
        }
        else
            sequence_length++;
    }

    if (sequence_length > 0) {
        current_sequence[sequence_length] = '\0';

        if (*outputSize == capacity)
        {
            capacity *= 2;
            output = realloc(output, capacity * sizeof(int));
        }

        if (red_black_tree_find_node_by_value(current_sequence, dictionary->root) == NULL) {
            red_black_tree_insert(dictionary, code, current_sequence);
            output[*outputSize] = code;
        }
        else
            output[*outputSize] = red_black_tree_node_get_code(red_black_tree_find_node_by_value(current_sequence, dictionary->root));
        (*outputSize)++;
    }
    return output;
}


unsigned char* lzw_decompress_encode(const int* compressed, int compressed_size, RBTree* dictionary)
{
    int capacity = OUT_CAPACITY;
    unsigned char* decompressed = calloc(capacity, sizeof(unsigned char));

    int decompressed_size = 0;
    int code = 0;
    unsigned char* entry;


    for (int i = 0; i < compressed_size; i++)
    {
        code = compressed[i];
        RBTreeNode* tmpValueNode =  red_black_tree_find_by_code(dictionary, code);
        if(tmpValueNode == NULL)
        {
           fprintf(stderr,"Poskodeny subor!!");
           return NULL;
        }
        entry = red_black_tree_node_get_value(tmpValueNode);

        if (decompressed_size + strlen(entry) >= capacity)
        {
            capacity *= 2;
            unsigned char* temp = realloc(decompressed, capacity);

            decompressed = temp;
        }

        strcat(decompressed, entry);
        decompressed_size += strlen(entry);
    }

    return decompressed;
}
