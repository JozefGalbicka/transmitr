//
// Created by dominik on 1/1/24.
//

#ifndef TRANSMITR_CODE_TABLE_H
#define TRANSMITR_CODE_TABLE_H

#include "code_table_node.h"

typedef struct CodeTable
{
    CodeNode* array;
}CodeTable;

void code_table_init(CodeTable* this);
void code_table_set_code(CodeTable* this, unsigned char data, const char* code);
CodeNode* code_table_get_node(CodeTable* this, unsigned char data);
void code_table_destroy(CodeTable* this);
int code_table_get_size(CodeTable* this);


#endif // TRANSMITR_CODE_TABLE_H
