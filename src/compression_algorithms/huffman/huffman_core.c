//
// Created by dominik on 1/1/24.
//

#include "huffman_core.h"
#include <string.h>

#define MAX_SYMBOLS 256
#define EMPTY 0

long long wrapper_min_heap_node_get_freq(void* node)
{
    return min_heap_node_get_freq((MinHeapNode*) node);
}

void wrapper_min_heap_node_init(void* node, unsigned char data, long long freq)
{
    return min_heap_node_init((MinHeapNode*) node, data, freq );
}

void wrapper_min_heap_node_destroy(void* node)
{
    return min_heap_node_destroy((MinHeapNode*) node);
}

static void encodeToBitstream(CodeTable* table, unsigned char* input, unsigned char* output, int *outputSize, int *validBitsInLastByte)
{
    int outputBitPos = EMPTY;
    int outputBytePos = EMPTY;
    output[0] = EMPTY;

    unsigned char c;
    while ((c = *input++) != '\0')
    {
        char* code = code_node_get_code(code_table_get_node(table, c));
        if (code)
        {
            for (int i = 0; code[i] != '\0'; i++)
            {
                if (code[i] == '1')
                    output[outputBytePos] |= (1 << (7 - outputBitPos));
                outputBitPos++;
                if (outputBitPos == 8)
                {
                    outputBitPos = EMPTY;
                    outputBytePos++;
                    output[outputBytePos] = EMPTY;
                }
            }
        }
    }

    *outputSize = outputBytePos;
    if(outputBitPos != 0)
        *outputSize += 1;

    *validBitsInLastByte = (outputBitPos == 0) ? 8 : outputBitPos;
}

static void decodeFromBitstream(const unsigned char* input, int inputSize, int validBitsInLastByte, CodeTable* table, unsigned char* output)
{
    if (!input || !table || !output)
        return;

    int outputIndex = EMPTY;
    int inputBitPos = EMPTY;
    int inputBytePos = EMPTY;
    char currentCode[MAX_SYMBOLS] = {0};
    int currentCodeLength = EMPTY;

    while (inputBytePos < inputSize) {
        if (inputBytePos == inputSize - 1 && inputBitPos >= validBitsInLastByte)
            break;

        currentCode[currentCodeLength++] = (input[inputBytePos] & (1 << (7 - inputBitPos))) ? '1' : '0';
        currentCode[currentCodeLength] = '\0';

        for (int i = 0; i < MAX_SYMBOLS; i++) {

            CodeNode *node = code_table_get_node(table, i);
            const char *code = code_node_get_code(node);

            if (strcmp(code, currentCode) == 0)
            {
                output[outputIndex++] = i;
                currentCodeLength = EMPTY;
                break;
            }
        }

        inputBitPos++;
        if (inputBitPos == 8)
        {
            inputBitPos = 0;
            inputBytePos++;
        }
    }

    output[outputIndex] = '\0';
}


void huffman_encode_from_file( unsigned char* input, unsigned char* output,int* outputSize,int* validBitsInLastByte, CodeTable *codeTable)
{


    FrTable frTable;
    frequency_table_init(&frTable);
    frequency_table_create(&frTable, input);

    MinHeap minHeap;
    min_heap_init(&minHeap, sizeof(MinHeapNode));

    for(int i = 0; i < MAX_SYMBOLS; i++)
    {
        if (frequency_table_get_frequency(&frTable, i))
        {
            min_heap_add(&minHeap, i, frequency_table_get_frequency(&frTable, i) , wrapper_min_heap_node_get_freq, wrapper_min_heap_node_init, wrapper_min_heap_node_destroy);
        }
    }

    frequency_table_destroy(&frTable);

    HuffmanTree huftree;
    huffman_tree_init(&huftree, &minHeap);

    min_heap_destroy(&minHeap, wrapper_min_heap_node_destroy);

    huffman_tree_create(&huftree);
    huffman_tree_generate_codes(&huftree, codeTable);

    min_heap_destroy(&minHeap, wrapper_min_heap_node_destroy);

    encodeToBitstream(codeTable, input, output, outputSize, validBitsInLastByte);

    huffman_tree_destroy(&huftree);

}

void huffman_decode(const unsigned char* input, int inputSize, int validBitsInLastByte, CodeTable* codeTable, unsigned char* output)
{
    decodeFromBitstream(input, inputSize, validBitsInLastByte, codeTable, output);
}


