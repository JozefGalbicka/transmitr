//
// Created by dominik on 1/3/24.
//

#ifndef TRANSMITR_RED_BLACK_TREE_NODE_H
#define TRANSMITR_RED_BLACK_TREE_NODE_H

typedef enum Colour
{
    red,
    black,
}Colour;

typedef struct RBTreeNode
{
    struct RBTreeNode* parent;
    struct RBTreeNode* left;
    struct RBTreeNode* right;
    unsigned char* value;
    int code;
    Colour nodeColour;

}RBTreeNode;

void red_black_tree_node_init(RBTreeNode* this);
void red_black_tree_node_destroy(RBTreeNode* this);

void red_black_tree_node_set_left_son(RBTreeNode* this, RBTreeNode* left);
void red_black_tree_node_set_right_son(RBTreeNode* this,RBTreeNode* right);
void red_black_tree_node_set_parent(RBTreeNode* this,RBTreeNode* parent);

RBTreeNode* red_black_tree_node_get_left_son(RBTreeNode* this);
RBTreeNode* red_black_tree_node_get_right_son(RBTreeNode* this);
RBTreeNode* red_black_tree_node_get_parent(RBTreeNode* this);
RBTreeNode* red_black_tree_get_uncle(RBTreeNode* this);

void red_black_tree_node_recolour(RBTreeNode* this);

void red_black_tree_node_set_colour(RBTreeNode* this, Colour nodeColour);
void red_black_tree_node_set_code(RBTreeNode* this, int code);
void red_black_tree_node_set_value(RBTreeNode* this, unsigned char* value); //plytka kopia

Colour red_black_tree_node_get_colour(RBTreeNode* this);
int red_black_tree_node_get_code(RBTreeNode* this);
unsigned char* red_black_tree_node_get_value(RBTreeNode* this);

#endif // TRANSMITR_RED_BLACK_TREE_NODE_H
