//
// Created by dominik on 1/3/24.
//

#ifndef TRANSMITR_RED_BLACK_TREE_H
#define TRANSMITR_RED_BLACK_TREE_H

#include "red_black_tree_node.h"
#include <stdlib.h>

typedef struct RBTree
{
    RBTreeNode* root;
    size_t size;
}RBTree;

void red_black_tree_init(RBTree* this);
void red_black_tree_destroy(RBTree* this);

void red_black_tree_insert(RBTree* this,int code, const unsigned char* value);

RBTreeNode* red_black_tree_find_node_by_value(unsigned char* value, RBTreeNode* subRoot);

RBTreeNode* red_black_tree_remove(RBTree* this, RBTreeNode* node);
RBTreeNode* red_black_tree_find_by_code(RBTree* this, int code);


#endif // TRANSMITR_RED_BLACK_TREE_H
