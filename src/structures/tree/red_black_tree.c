//
// Created by dominik on 1/3/24.
//
#include "red_black_tree.h"
#define EMPTY 0

void red_black_tree_init(RBTree* this)
{
    this->root = NULL;
    this->size = EMPTY;
}



void red_black_tree_destroy(RBTree* this)
{
    red_black_tree_postorder(this->root, red_black_tree_node_destroy);
    this->root = NULL;
    this->size = EMPTY;
}


void red_black_tree_postorder(RBTreeNode* node, void (*node_function)(RBTreeNode*))
{
    if (node != NULL)
    {
        red_black_tree_postorder(red_black_tree_node_get_left_son(node), node_function);
        red_black_tree_postorder(red_black_tree_node_get_right_son(node), node_function);
        node_function(node);
    }
}


void red_black_tree_left_rotation(RBTree* this, RBTreeNode* node)
{
    if(node == NULL || red_black_tree_node_get_right_son(node) == NULL)
        return;

    RBTreeNode* newTop = red_black_tree_node_get_right_son(node);
    red_black_tree_node_set_right_son(node,red_black_tree_node_get_left_son(newTop));

    if(red_black_tree_node_get_left_son(newTop) != NULL)
        red_black_tree_node_set_parent(red_black_tree_node_get_left_son(newTop), node);

    red_black_tree_node_set_parent(newTop, red_black_tree_node_get_parent(node));

    if(red_black_tree_node_get_parent(node) != NULL)
    {
        if(red_black_tree_node_get_left_son(red_black_tree_node_get_parent(node)) == node)
            red_black_tree_node_set_left_son(red_black_tree_node_get_parent(node), newTop);
        else
            red_black_tree_node_set_right_son(red_black_tree_node_get_parent(node), newTop);
    }
    else
        this->root = newTop;

    red_black_tree_node_set_left_son(newTop, node);
    red_black_tree_node_set_parent(node, newTop);
}


void red_black_tree_right_rotation(RBTree* this, RBTreeNode* node)
{
    if (node == NULL || red_black_tree_node_get_left_son(node) == NULL)
        return;

    RBTreeNode *newTop = red_black_tree_node_get_left_son(node);
    red_black_tree_node_set_left_son(node, red_black_tree_node_get_right_son(newTop));

    if (red_black_tree_node_get_right_son(newTop) != NULL)
        red_black_tree_node_set_parent(red_black_tree_node_get_right_son(newTop), node);

    red_black_tree_node_set_parent(newTop, red_black_tree_node_get_parent(node));

    if (red_black_tree_node_get_parent(node) != NULL) {
        if (red_black_tree_node_get_left_son(red_black_tree_node_get_parent(node)) == node)
            red_black_tree_node_set_left_son(red_black_tree_node_get_parent(node), newTop);
        else
            red_black_tree_node_set_right_son(red_black_tree_node_get_parent(node), newTop);
    } else
        this->root = newTop;

    red_black_tree_node_set_right_son(newTop, node);
    red_black_tree_node_set_parent(node, newTop);
}

/*

void red_black_tree_insert(RBTree* this,int code,unsigned char* value)
{

}

RBTreeNode* red_black_tree_get_node(RBTree* this, unsigned char* value)
{

}

RBTreeNode* red_black_tree_remove(RBTree* this, RBTreeNode* node)
{

}*/