//
// Created by dominik on 1/1/24.
//

#include "code_table_node.h"
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
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

void code_node_print(CodeNode* this) 
{
    printf("'%c'\t", this->data);
    printf("'%d'\t", this->code_size);
    printf("'%.*s'\n", this->code_size, this->code);
}

unsigned char *code_node_serialize(CodeNode *this, size_t *size) 
{
    uint16_t len = htons(this->code_size);
    size_t buffer_size = sizeof(this->data) + sizeof(len) + this->code_size;
    unsigned char *buffer = malloc(buffer_size);

    memcpy(buffer, &len, sizeof(len));
    memcpy(buffer + sizeof(len), &this->data, sizeof(this->data));
    memcpy(buffer + sizeof(len) + sizeof(this->data), this->code, this->code_size);

    *size = buffer_size;

    return buffer;
}

unsigned char *code_node_deserialize(CodeNode *this, unsigned char *buffer, size_t size)
{
    uint16_t len;
    memcpy(&len, buffer, sizeof(len));
    this->code_size = ntohs(len);
    memcpy(&this->data, buffer + sizeof(len), sizeof(this->data));
    this->code = malloc(this->code_size);
    memcpy(this->code, buffer + sizeof(len) + sizeof(this->data), this->code_size);

    return (buffer + sizeof(len) + sizeof(this->data) + this->code_size - 1);
}
