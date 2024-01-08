//
// Created by dominik on 1/1/24.
//

#ifndef TRANSMITR_CODE_TABLE_NODE_H
#define TRANSMITR_CODE_TABLE_NODE_H

#include <stdlib.h>

typedef struct CodeNode
{
    unsigned char data;
    short code_size;
    char* code;

}CodeNode;

void code_node_init(CodeNode* this, unsigned char data);
void code_node_set_code(CodeNode* this, const char* code, short code_size);
void code_node_destroy(CodeNode* this);
char* code_node_get_code(CodeNode* this);
short code_node_get_code_size(CodeNode* this);

unsigned char *code_node_serialize(CodeNode *this, size_t *size);
unsigned char *code_node_deserialize(CodeNode *this, unsigned char *buffer, size_t size);

#endif // TRANSMITR_CODE_TABLE_NODE_H
