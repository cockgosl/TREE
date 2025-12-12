#include "my_math.h"

double TREE_SOLVE (TREE_t* tree, NODE_t* node) {
    assert (tree);
    assert (node);

    double solution = NAN;
    if (node->type == OP_T) {
        switch (node->data.op_name) {
            case (MUL):
                solution = TREE_SOLVE(tree, node->left) * TREE_SOLVE(tree, node->right);
                break;
            case (ADD):
                solution = TREE_SOLVE(tree, node->left) + TREE_SOLVE(tree, node->right);
                break;
            case (SUB):
                solution = TREE_SOLVE(tree, node->left) - TREE_SOLVE(tree, node->right);
                break;
            case (DIV) :
                solution = TREE_SOLVE(tree, node->left) / TREE_SOLVE(tree, node->right);
                break;
            case (POW):
                solution = pow(TREE_SOLVE(tree, node->left), TREE_SOLVE(tree, node->right));
                break;
            case (SIN):
                solution = sin(TREE_SOLVE(tree, node->left));
                break;
            case (COS):
                solution = cos(TREE_SOLVE(tree, node->left));
                break;
            case (TG) :
                solution = tan(TREE_SOLVE(tree, node->left));
                break;
            case (LOG) :
                solution = log(TREE_SOLVE(tree, node->left));
                break;
        }
    }
    else if (node->type == NUM_T) {
        solution = node->value.value;
    }
    else {
        solution = tree->variables[node->data.number];
    }
    return solution;
}

NODE_t* TREE_DIFFERENTIATE (TREE_t tree, NODE_t* node, char variable) {
    NODE_t* new_node = (NODE_t*)calloc (1, sizeof (NODE_t));

    assert (new_node);
    if (node->type == NUM_T || (node->type == VAR_T && node->value.name != variable)) {
        new_node->type = NUM_T;
        new_node->value.value = 0;
    }
    else if (node->type == 2 && node->value.name == variable) {
        new_node->type = NUM_T;
        new_node->value.value = 1;
    }
    else {
        switch (node->data.op_name) {
            case (MUL):
                MUL_DIF (tree, node, new_node, variable);
                break;
            case (ADD):
                ADD_DIF (tree, node, new_node, variable);
                break;
            case (SUB):
                SUB_DIF (tree, node, new_node, variable);
                break;
            case (DIV) :
                DIV_DIF (tree, node, new_node, variable);
                break;
            case (POW):
                POW_DIF (tree, node, new_node, variable);
                break;
            case (SIN):
                SIN_DIF (tree, node, new_node, variable);
                break;
            case (COS):
                COS_DIF (tree, node, new_node, variable);
                break;
            case (TG) :
                TG_DIF (tree, node, new_node, variable);
                break;
            case (LOG) :
                LOG_DIF (tree, node, new_node, variable);
                break;
        }
    }
    if (new_node->left) {
        new_node->left->prev = new_node;
    }
    if (new_node->right) {
        new_node->right->prev = new_node;
    }

    return new_node;
}

void MUL_DIF (TREE_t tree, NODE_t* node, NODE_t* new_node, char variable) {
    new_node->type = OP_T;
    new_node->data.op_name = ADD;
    new_node->left = (NODE_t*)calloc(1, sizeof(NODE_t));
    assert(new_node->left);
    new_node->right = (NODE_t*)calloc(1, sizeof(NODE_t));
    assert(new_node->right);

    new_node->left->type = OP_T;
    new_node->left->data.op_name = MUL;
    new_node->left->left = TREE_DIFFERENTIATE (tree, node->left, variable);
    new_node->left->right = COPY_NODE(node->right);

    new_node->right->type = OP_T;
    new_node->right->data.op_name = MUL;
    new_node->right->right = TREE_DIFFERENTIATE (tree, node->right, variable);
    new_node->right->left = COPY_NODE(node->left);
}

void ADD_DIF (TREE_t tree, NODE_t* node, NODE_t* new_node, char variable) {
    new_node->type = OP_T;
    new_node->data.op_name = ADD;
    new_node->left = TREE_DIFFERENTIATE (tree, node->left, variable);
    new_node->right = TREE_DIFFERENTIATE (tree, node->right, variable);
}

void SUB_DIF (TREE_t tree, NODE_t* node, NODE_t* new_node, char variable) {
    new_node->type = OP_T;
    new_node->data.op_name = SUB;
    new_node->left = TREE_DIFFERENTIATE (tree, node->left, variable);
    new_node->right = TREE_DIFFERENTIATE (tree, node->right, variable);
}
void DIV_DIF (TREE_t tree, NODE_t* node, NODE_t* new_node, char variable) {
    new_node->type = OP_T;
    new_node->data.op_name = DIV;
    new_node->left = (NODE_t*)calloc(1, sizeof(NODE_t));
    assert(new_node->left);
    new_node->right = (NODE_t*)calloc(1, sizeof(NODE_t));
    assert(new_node->right);

    MUL_DIF (tree, node, new_node->left, variable);
    new_node->left->data.op_name = SUB;

    new_node->right->type = OP_T;
    new_node->right->data.op_name = POW;
    new_node->right->right = (NODE_t*)calloc(1, sizeof(NODE_t));
    assert(new_node->right->right);
    new_node->right->right->type = NUM_T;
    new_node->right->right->value.value = 2;
    new_node->right->left = COPY_NODE(node->right);
}

void LOG_DIF (TREE_t tree, NODE_t* node, NODE_t* new_node, char variable) {
    new_node->type = OP_T;
    new_node->data.op_name = DIV;
    
    new_node->left = TREE_DIFFERENTIATE(tree, node->left, variable);

    new_node->right = COPY_NODE(node->left);
}

void POW_DIF (TREE_t tree, NODE_t* node, NODE_t* new_node, char variable) {
    new_node->type = OP_T;
    new_node->data.op_name = MUL;

    new_node->left = (NODE_t*)calloc(1, sizeof(NODE_t));
    assert(new_node->left);
    new_node->left->type = OP_T;
    new_node->left->data.op_name = POW;

    new_node->left->left = (NODE_t*)calloc(1, sizeof(NODE_t));
    assert(new_node->left->left);
    new_node->left->left->type = NUM_T;
    new_node->left->left->value.value = M_E;

    new_node->left->right = (NODE_t*)calloc(1, sizeof(NODE_t));
    assert(new_node->left->right);
    new_node->left->right->type = OP_T;
    new_node->left->right->data.op_name = MUL;
    new_node->left->right->right = COPY_NODE(node->right);
    assert(new_node->left->right->right);
    new_node->left->right->left = (NODE_t*)calloc(1, sizeof(NODE_t));
    assert(new_node->left->right->left);
    new_node->left->right->left->type = OP_T;
    new_node->left->right->left->data.op_name = LOG;
    new_node->left->right->left->left = COPY_NODE(node->left);
    assert(new_node->left->right->left->left);

    new_node->right = (NODE_t*)calloc(1, sizeof(NODE_t));
    assert(new_node->right);

    MUL_DIF (tree, new_node->left->right, new_node->right, variable);
}

void SIN_DIF (TREE_t tree, NODE_t* node, NODE_t* new_node, char variable) {
    new_node->type = OP_T;
    new_node->data.op_name = COS;
    new_node->left = COPY_NODE(node->left);
}

void COS_DIF (TREE_t tree, NODE_t* node, NODE_t* new_node, char variable) {
    new_node->type = OP_T;
    new_node->data.op_name = MUL;
    new_node->left = (NODE_t*)calloc(1, sizeof(NODE_t));
    new_node->right = (NODE_t*)calloc(1, sizeof(NODE_t));

    new_node->left->type = OP_T;
    new_node->left->data.op_name = SIN;
    new_node->left->left = COPY_NODE(node->left);

    new_node->right->type = NUM_T;
    new_node->right->value.value = -1;
}

void TG_DIF (TREE_t tree, NODE_t* node, NODE_t* new_node, char variable) {
    new_node->type = OP_T;
    new_node->data.op_name = MUL;
    new_node->right = TREE_DIFFERENTIATE (tree, node->left, variable);
    new_node->left = (NODE_t*)calloc(1, sizeof(NODE_t));
    assert(new_node->left);

    new_node->left->type = OP_T;
    new_node->left->data.op_name = DIV;
    new_node->left->left = (NODE_t*)calloc(1, sizeof(NODE_t));
    assert(new_node->left->left);
    new_node->left->left->type = NUM_T;
    new_node->left->left->value.value = 1;
    new_node->left->right = (NODE_t*)calloc(1, sizeof(NODE_t));
    assert(new_node->left->right);
    new_node->left->right->type = OP_T;
    new_node->left->right->data.op_name = POW;
    new_node->left->right->left = (NODE_t*)calloc(1, sizeof(NODE_t));
    assert(new_node->left->right->left);
    new_node->left->right->left->type = OP_T;
    new_node->left->right->left->data.op_name = COS;
    new_node->left->right->left->left = COPY_NODE(node->left);
    new_node->left->right->right = (NODE_t*)calloc(1, sizeof(NODE_t));
    assert(new_node->left->right->right);
    new_node->left->right->right->type = NUM_T;
    new_node->left->right->right->value.value = 2;
}