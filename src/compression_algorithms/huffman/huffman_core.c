//
// Created by dominik on 1/1/24.
//

#include "huffman_core.h"


#define MAX_SYMBOLS 256
#define EMPTY 0

/**
 * Obaľovacia fukncia pre fukciu ktorá získava frekvenciu pre uzol, node min-heapu.
 *
 * @param node Ukazovateľ na uzol, z ktorého sa má získať frekvencia.
 * @return Frekvencia daného uzla.
 */
long long wrapper_min_heap_node_get_freq(void* node)
{
    return min_heap_node_get_freq((MinHeapNode*) node);
}

/**
 * Obaľovacia fukncia pre fukciu konštruktora uzla, node min-heap.
 *
 * @param node Ukazovateľ na uzol, ktorý sa má inicializovať.
 * @param data Dáta, ktoré sa majú uložiť do uzla.
 * @param freq Frekvencia, ktorá sa má priradiť uzlu.
 */
void wrapper_min_heap_node_init(void* node, unsigned char data, long long freq)
{
    return min_heap_node_init((MinHeapNode*) node, data, freq );
}

/**
 * Obaľovacia fukncia pre fukciu deštruktora uzla, node min-heap.
 *
 * @param node Ukazovateľ na uzol, ktorý sa má zničiť.
 */
void wrapper_min_heap_node_destroy(void* node)
{
    return min_heap_node_destroy((MinHeapNode*) node);
}

/**
 * Statická fukcia ktorá kóduje vstupné dáta do bitového prúdu (bitstream) pomocou tabuľky kódov.
 * Táto funkcia prechádza vstupné dáta a pre každý bajt vstupu vyhľadá príslušný kódv tabuľke kódov.
 * Každý takýto kód je potom zapísaný do výstupného bitového prúdu.
 *
 * @param table Ukazovateľ na CodeTable, obsahujúcu kódovaciu tabuľku.
 * @param input Ukazovateľ na vstupné dáta, ktoré majú byť zakódované.
 * @param inputSize Veľkosť vstupných dát v bajtoch.
 * @param output Ukazovateľ na pole bajtov, kde budú uložené zakódované dáta.
 * @param outputSize Ukazovateľ na premennú, kde bude uložená veľkosť výstupných dát.
 * @param validBitsInLastByte Ukazovateľ na premennú, kde bude uložený počet platných bitov v poslednom bajte výstupného bitového prúdu.
 */
static void encodeToBitstream(CodeTable* table, unsigned char* input, size_t inputSize, unsigned char* output, int *outputSize, int *validBitsInLastByte)
{
    int outputBitPos = EMPTY;
    int outputBytePos = EMPTY;
    output[0] = EMPTY;

    unsigned char c;
    for (size_t z = 0; z < inputSize; z++)
    {
        c = *input++;
        char* code = code_node_get_code(code_table_get_node(table, c));
        if (code)
        {
            short codeSize = code_node_get_code_size(code_table_get_node(table, c));

            for (int i = 0; i < codeSize; i++)
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

/**
 * Statická funkcia ktorá dekóduje bitový prúd späť do pôvodných dát s použitím tabuľky kódov.
 * Táto funkcia prechádza bitový prúd vstupných dát, rozkladá ho na individuálne kódya hľadá zodpovedajúce znaky v tabuľke kódov.
 * Každý nájdený kód sa prevádza späť na pôvodný znak a ukladá do výstupného poľa.
 *
 * @param input Ukazovateľ na vstupný bitový prúd na dekódovanie.
 * @param inputSize Veľkosť vstupného bitového prúdu v bajtoch.
 * @param validBitsInLastByte Počet platných bitov v poslednom bajte vstupného bitového prúdu.
 * @param table Ukazovateľ na CodeTable, obsahujúcu kódovaciu tabuľku.
 * @param output Ukazovateľ na výstupné pole bajtov, kde budú uložené dekódované dáta.
 * @param outputSize Ukazovateľ na premennú, kde bude uložená veľkosť výstupných dekódovaných dát.
 */
static void decodeFromBitstream(const unsigned char* input, int inputSize, int validBitsInLastByte, CodeTable* table, unsigned char* output, int* outputSize)
{
    if (!input || !table || !output)
        return;

    int outputIndex = EMPTY;
    int inputBitPos = EMPTY;
    int inputBytePos = EMPTY;
    char currentCode[MAX_SYMBOLS ] = {0};
    short currentCodeLength = EMPTY;


    while (inputBytePos < inputSize)
    {
        if (inputBytePos == inputSize - 1 && inputBitPos >= validBitsInLastByte)
            break;

        currentCode[currentCodeLength++] = (input[inputBytePos] & (1 << (7 - inputBitPos))) ? '1' : '0';

      for (int i = 0; i < MAX_SYMBOLS; i++)
        {

            CodeNode *node = code_table_get_node(table, i);
            const char *code = code_node_get_code(node);
            short codeSize = code_node_get_code_size(node);

            if(currentCodeLength == codeSize)
            {
                short u;
                for(u = 0; u < codeSize;u++)
                {
                    if(currentCode[u] != code[u])
                        break;
                }

                if (u == codeSize)
                {
                    output[outputIndex++] = i;
                    currentCodeLength = EMPTY;
                    break;
                }

            }
        }

        inputBitPos++;
        if (inputBitPos == 8)
        {
            inputBitPos = 0;
            inputBytePos++;
        }
    }
    *outputSize = outputIndex;

}

/**
 * Zakóduje vstupné dáta pomocou Huffmanovho kódovania.
 * Táto funkcia najprv vytvorí frekvenčnú tabuľku na základe vstupných dát, potom vytvorí minimálnu haldu a Huffmanov strom.
 * Po vytvorení Huffmanovho stromu sa generujú kódy pre každý znak a tieto kódy sa používajú na zakódovanie vstupných dát do bitového prúdu.
 *
 * @param input Ukazovateľ na vstupné dáta, ktoré majú byť zakódované.
 * @param inputSize Veľkosť vstupných dát v bajtoch.
 * @param output Ukazovateľ na pole bajtov, kde budú uložené zakódované dáta.
 * @param outputSize Ukazovateľ na premennú, kde bude uložená veľkosť výstupných dát.
 * @param validBitsInLastByte Ukazovateľ na premennú, kde bude uložený počet platných bitov v poslednom bajte výstupného bitového prúdu.
 * @param codeTable Ukazovateľ na CodeTable použitú pre zakódovanie znakov.
 */
void huffman_encode( unsigned char* input, size_t inputSize,unsigned char* output,int* outputSize,int* validBitsInLastByte, CodeTable *codeTable)
{


    FrTable frTable;
    frequency_table_init(&frTable);
    frequency_table_create(&frTable, input, inputSize);

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

    encodeToBitstream(codeTable, input, inputSize, output, outputSize, validBitsInLastByte);

    huffman_tree_destroy(&huftree);

}

/**
 * Dekóduje vstupné dáta zakódované pomocou Huffmanovho kódovania.
 * Táto funkcia používa statickú funkciu decodeFromBitstream.
 * Slúži ako obaľovacia fukcia pre ňu.
 *
 * @param input Ukazovateľ na vstupný bitový prúd na dekódovanie.
 * @param inputSize Veľkosť vstupného bitového prúdu v bajtoch.
 * @param validBitsInLastByte Počet platných bitov v poslednom bajte vstupného bitového prúdu.
 * @param codeTable Ukazovateľ na CodeTable, obsahujúcu kódovaciu tabuľku.
 * @param output Ukazovateľ na výstupné pole bajtov, kde budú uložené dekódované dáta.
 * @param outputSize Ukazovateľ na premennú, kde bude uložená veľkosť výstupných dekódovaných dát.
 */
void huffman_decode(const unsigned char* input, int inputSize, int validBitsInLastByte, CodeTable* codeTable, unsigned char* output, int* outputSize)
{
    decodeFromBitstream(input, inputSize, validBitsInLastByte, codeTable, output, outputSize);
}


