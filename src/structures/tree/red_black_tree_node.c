//
// Created by dominik on 1/3/24.
//

#include "red_black_tree_node.h"
#include <stdio.h>
#include <stdlib.h>

#define EMPTY 0

void red_black_tree_node_init(RBTreeNode* this)
{
    this->value = NULL;
    this->nodeColour = black;
    this->code = EMPTY;
    this->parent = NULL;
    this->left = NULL;
    this->right = NULL;
}


void red_black_tree_node_destroy(RBTreeNode* this)
{
    if(this->value != NULL)
        free(this->value);

    this->value = NULL;
    this->nodeColour = black;
    this->code = EMPTY;
    this->parent = NULL;
    this->left = NULL;
    this->right = NULL;
}


void red_black_tree_node_set_left_son(RBTreeNode* this, RBTreeNode* left)
{
    this->left = left;
}


void red_black_tree_node_set_right_son(RBTreeNode* this,RBTreeNode* right)
{
    this->right = right;
}


void red_black_tree_node_set_parent(RBTreeNode* this,RBTreeNode* parent)
{
    this->parent = parent;
}


RBTreeNode* red_black_tree_node_get_left_son(RBTreeNode* this)
{
    return this->left;
}


RBTreeNode* red_black_tree_node_get_right_son(RBTreeNode* this)
{
    return this->right;
}


RBTreeNode* red_black_tree_node_get_parent(RBTreeNode* this)
{
    return this->parent;
}


RBTreeNode* red_black_tree_get_uncle(RBTreeNode* this)
{
    if(this->parent == NULL)
        return NULL;

    if(this->parent->parent == NULL)
        return NULL;

    return ((this->parent->parent->right == this)?(this->parent->parent->left):(this->parent->parent->right));
}

void red_black_tree_node_recolour(RBTreeNode* this)
{
    this->nodeColour = (this->nodeColour == red) ? black : red;
}

void red_black_tree_node_set_colour(RBTreeNode* this, Colour nodeColour)
{
    this->nodeColour = nodeColour;
}


void red_black_tree_node_set_code(RBTreeNode* this, int code)
{
    this->code = code;
}


void red_black_tree_node_set_value(RBTreeNode* this, unsigned char* value)
{
    if(this->value != NULL)
        free(this->value);

    this->value = value;
}


Colour red_black_tree_node_get_colour(RBTreeNode* this)
{
    return this->nodeColour;
}


int red_black_tree_node_get_code(RBTreeNode* this)
{
    return this->code;
}


unsigned char* red_black_tree_node_get_value(RBTreeNode* this)
{
    return this->value;
}