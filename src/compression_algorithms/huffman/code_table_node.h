//
// Created by dominik on 1/1/24.
//

#ifndef TRANSMITR_CODE_TABLE_NODE_H
#define TRANSMITR_CODE_TABLE_NODE_H

#include <stdlib.h>

typedef struct CodeNode
{
    unsigned char data;
    char* code;
}CodeNode;

void code_node_init(CodeNode* this, unsigned char data);
void code_node_set_code(CodeNode* this, const char* code);
void code_node_destroy(CodeNode* this);
char* code_node_get_code(CodeNode* this);

#endif // TRANSMITR_CODE_TABLE_NODE_H
