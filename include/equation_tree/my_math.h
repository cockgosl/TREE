#include "tree_shape.h"

double TREE_SOLVE (TREE_t* tree, NODE_t* node);
NODE_t* TREE_DIFFERENTIATE (TREE_t tree, NODE_t* node, char variable);

void MUL_DIF (TREE_t tree, NODE_t* node, NODE_t* new_node, char variable);
void ADD_DIF (TREE_t tree, NODE_t* node, NODE_t* new_node, char variable);
void SUB_DIF (TREE_t tree, NODE_t* node, NODE_t* new_node, char variable);
void DIV_DIF (TREE_t tree, NODE_t* node, NODE_t* new_node, char variable);
void POW_DIF (TREE_t tree, NODE_t* node, NODE_t* new_node, char variable);
void SIN_DIF (TREE_t tree, NODE_t* node, NODE_t* new_node, char variable);
void COS_DIF (TREE_t tree, NODE_t* node, NODE_t* new_node, char variable);
void TG_DIF (TREE_t tree, NODE_t* node, NODE_t* new_node, char variable);


