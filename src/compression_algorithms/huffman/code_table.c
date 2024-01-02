//
// Created by dominik on 1/1/24.
//

#include "code_table.h"
#include <stdlib.h>
#define MAX_SYMBOLS 256

void code_table_init(CodeTable* this)
{
    this->array = calloc(MAX_SYMBOLS, sizeof(CodeNode));
    for (int i = 0; i < MAX_SYMBOLS; i++)
    {
        code_node_init((CodeNode*)((unsigned char*)this->array + i*sizeof(CodeNode)), i);
    }
}

void code_table_set_code(CodeTable* this, unsigned char data, const char* code)
{
    code_node_set_code((CodeNode*)((unsigned char*)this->array + data*sizeof(CodeNode)), code);
}

CodeNode* code_table_get_node(CodeTable* this, unsigned char data)
{
    return (CodeNode*)((unsigned char*)this->array+ data*sizeof(CodeNode));
}

void code_table_destroy(CodeTable* this)
{
    for (int i = 0; i < MAX_SYMBOLS; i++)
    {
        code_node_destroy((CodeNode*)((unsigned char*)this->array + i*sizeof(CodeNode)));
    }
    free(this->array);
}

int code_table_get_size(CodeTable* this)
{
    return MAX_SYMBOLS;
}

