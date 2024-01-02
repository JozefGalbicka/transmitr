//
// Created by dominik on 1/1/24.
//

#include "code_table_node.h"
#include <stdlib.h>
#include <string.h>

void code_node_init(CodeNode* this, unsigned char data)
{
    this->data = data;
    this->code = NULL;
    code_node_set_code(this, "");
}

void code_node_set_code(CodeNode* this, const char* code)
{
    free(this->code);
    this->code = strdup(code);
}

void code_node_destroy(CodeNode* this)
{
    free(this->code);
}

char* code_node_get_code(CodeNode* this)
{
    return this->code;
}






