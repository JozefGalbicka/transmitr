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
void red_black_tree_postorder(RBTreeNode* node, void (*node_function)(RBTreeNode*));
void red_black_tree_left_rotation(RBTree* this, RBTreeNode* node);
void red_black_tree_right_rotation(RBTree* this, RBTreeNode* node);

void red_black_tree_insert(RBTree* this,int code,unsigned char* value);

RBTreeNode* red_black_tree_get_node(RBTree* this, unsigned char* value);

RBTreeNode* red_black_tree_remove(RBTree* this, RBTreeNode* node);



#endif // TRANSMITR_RED_BLACK_TREE_H
