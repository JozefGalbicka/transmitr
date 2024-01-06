//
// Created by dominik on 1/1/24.
//

#include "code_table_node.h"
#include <stdlib.h>
#define EMPTY 0

void code_node_init(CodeNode* this, unsigned char data)
{
    this->code_size = EMPTY;
    this->data = data;
    this->code = NULL;
    code_node_set_code(this, "", EMPTY);
}

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

void code_node_destroy(CodeNode* this)
{
    free(this->code);
    this->code_size = EMPTY;
    this->code = NULL;
}

char* code_node_get_code(CodeNode* this)
{
    return this->code;
}

short code_node_get_code_size(CodeNode* this)
{
    return this->code_size;
}






