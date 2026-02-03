#include <stdio.h>
#include <stdlib.h>

#include "my_math.h"
#include "tree_shape.h"

void SYNTAX_ERROR();
NODE_t* GET_N(TREE_t tree, size_t* index);
NODE_t* GET_P(TREE_t tree, size_t* index);
NODE_t* GET_T(TREE_t tree, size_t* index);
NODE_t* GET_E(TREE_t tree, size_t* index);
NODE_t* GET_G(TREE_t tree, size_t* index);

int main() {
    TREE_t tree_test = {};
    size_t index = 0;


    TREE_INIT(&tree_test);

    FILE* input = fopen ("dump_inf/log/input.txt", "r");
    assert (input);
    TOKENS_C (input, &tree_test);


    fclose (input);

    tree_test.root = GET_G(tree_test, &index);

    FILE* output = fopen ("dump_inf/graphic/g.gv1", "wr");
    
    COMPLETE_PRINTG(tree_test.root, output);

    fclose (output);

    double answer = TREE_SOLVE (&tree_test, tree_test.root);
    printf ("the answer is %f\n", answer);

    TREE_DESTROY(tree_test);
}

NODE_t* GET_N (TREE_t tree, size_t* index) {

    NODE_t* val = tree.tokens + *index;
    (*index)++;

    return val;
}

NODE_t* GET_P (TREE_t tree, size_t* index) {
    NODE_t* val = NULL;

    if (tree.tokens[*index].data.op_name == '(') {

        (*index)++;
        val = GET_E(tree, index);
        if (tree.tokens[*index].data.op_name == ')') {
            (*index)++;
        }
        else {
            SYNTAX_ERROR();
        } 
    }
    else {

        val = GET_N(tree, index);
    }

    return val;
}

NODE_t* GET_T(TREE_t tree, size_t* index) {
    NODE_t* val = GET_P(tree, index);
    NODE_t* val1 = NULL;
    size_t op = *index;
    
    while (tree.tokens[*index].data.op_name == '*' || tree.tokens[*index].data.op_name == '/') {
        (*index)++;
        val1 = GET_P(tree, index);
        if (tree.tokens[op].data.op_name == '*') {
            tree.tokens[op].right = val1;
            tree.tokens[op].left = val;
            val = tree.tokens + op;
        }
        else if (tree.tokens[op].data.op_name == '/') {
            tree.tokens[op].right = val1;
            tree.tokens[op].left = val;
            val = tree.tokens + op;
        }
        op = *index;
    }

    return val;
}

NODE_t* GET_E(TREE_t tree, size_t* index) {
    NODE_t* val = GET_T(tree, index);
    NODE_t* val1 = NULL;
    size_t op = *index;
    
    while (tree.tokens[*index].data.op_name == '+' || tree.tokens[*index].data.op_name == '-') {
        (*index)++;
        val1 = GET_T(tree, index);
        if (tree.tokens[op].data.op_name == '+') {
            tree.tokens[op].right = val1;
            tree.tokens[op].left = val;
            val = tree.tokens + op;
        }
        else if (tree.tokens[op].data.op_name == '-') {
            tree.tokens[op].right = val1;
            tree.tokens[op].left = val;
            val = tree.tokens + op;
        }
        op = *index;
    }


    return val;
}

NODE_t* GET_G(TREE_t tree, size_t* index) {

    NODE_t* val = GET_E(tree, index);


    if (tree.tokens[*index].type == UNDEF) {
       return val;
    }
    else {
        SYNTAX_ERROR();
    }
    return val;
}

void TOKENS_C (FILE* input, TREE_t* tree) {
    size_t size = 0;
    size_t counter = 0;
    int val = 0;
    char* s = READ_BUFFER (input, &size);
    char* temp = s;
    while (*s != '\0' && counter < size) {
        counter++;
        if ('0' <= *s && *s <= '9') {
            while ('0' <= *s && *s <= '9') {
            val = val*10 + *s - '0';
            s += sizeof(char);
            }
            tree->tokens[tree->free[0]].type = NUM_T;
            tree->tokens[tree->free[0]].value.value = val;
            tree->tokens[tree->free[0]].number = tree->free[0];
            tree->free[0]++;
            val = 0;
        }
        else if (*s == '(') {
            s += sizeof(char);
            tree->tokens[tree->free[0]].type = OP_T;
            tree->tokens[tree->free[0]].data.op_name = FBRACKET;
            tree->tokens[tree->free[0]].number = tree->free[0];
            tree->free[0]++;
        }
        else if (*s == ')') {
            s += sizeof(char);
            tree->tokens[tree->free[0]].type = OP_T;
            tree->tokens[tree->free[0]].data.op_name = LBRACKET;
            tree->tokens[tree->free[0]].number = tree->free[0];
            tree->free[0]++;
        }
        else if (*s == '*') {
            s += sizeof(char);
            tree->tokens[tree->free[0]].type = OP_T;
            tree->tokens[tree->free[0]].data.op_name = MUL;
            tree->tokens[tree->free[0]].number = tree->free[0];
            tree->free[0]++;
        }
        else if (*s == '/') {
            s += sizeof(char);
            tree->tokens[tree->free[0]].type = OP_T;
            tree->tokens[tree->free[0]].data.op_name = DIV;
            tree->tokens[tree->free[0]].number = tree->free[0];
            tree->free[0]++;
        }
        else if (*s == '+') {
            s += sizeof(char);
            tree->tokens[tree->free[0]].type = OP_T;
            tree->tokens[tree->free[0]].data.op_name = ADD;
            tree->tokens[tree->free[0]].number = tree->free[0];
            tree->free[0]++;
        }
        else if (*s == '-') {
            s += sizeof(char);
            tree->tokens[tree->free[0]].type = OP_T;
            tree->tokens[tree->free[0]].data.op_name = SUB;
            tree->tokens[tree->free[0]].number = tree->free[0];
            tree->free[0]++;
        }
    }
    free (temp);
}

void SYNTAX_ERROR() {
    fprintf (stderr, "syntax error\n");
    abort();
}