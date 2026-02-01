#include <stdio.h>
#include <stdlib.h>

#include "my_math.h"
#include "tree_shape.h"

void SYNTAX_ERROR();
int GET_N(TREE_t tree);
int GET_P(TREE_t tree);
int GET_T(TREE_t tree);
int GET_E(TREE_t tree);
int GET_G(TREE_t tree);

char* s = "(5+8)/4";

int main() {
    TREE_t tree_test = {};

    TREE_INIT(&tree_test);

    int value = GET_G(tree_test);
    printf("the answer is %d\n", value);

    for (size_t i = 0; i < 4; i++) {
        printf ("number %ld = %d, op %ld = %d\n", i, tree_test.tokens[i].num, i , tree_test.tokens[i].op);
    }

    TREE_DESTROY(tree_test);
}

int GET_N (TREE_t tree) {
    int val = 0;
    size_t sign = 0;
    while ('0' <= *s && *s <= '9') {
        val = val*10 + *s - '0';
        sign++;
        s += sizeof(char);
    }

    if (sign == 0) {
        SYNTAX_ERROR();
    }

    tree.tokens[tree.free[0]].num = val;
    tree.free[0]++;

    return val;
}

int GET_P (TREE_t tree) {
    int val = 0;
    if (*s == '(') {
        s += sizeof(char);
        val = GET_E(tree);
        if (*s == ')') {
            s += sizeof(char);
        }
        else {
            SYNTAX_ERROR();
        } 
    }
    else {
        val = GET_N(tree);
    }
    return val;
}

int GET_T(TREE_t tree) {
    int val = GET_P(tree);
    int val1 = 0;
    char op = *s;
    
    while (*s == '*' || *s == '/') {
        s += sizeof(char);
        val1 = GET_P(tree);
        if (op == '*') {
            val *= val1;
            tree.tokens[tree.free[0]].op = MUL;
            tree.free[0]++;
        }
        else if (op == '/') {
            val /= val1;
            tree.tokens[tree.free[0]].op = DIV;
            tree.free[0]++;
        }
        op = *s;
    }


    return val;
}

int GET_E(TREE_t tree) {
    int val = GET_T (tree);
    int val1 = 0;
    char op = *s;
    while (*s == '+' || *s == '-') {
        s += sizeof(char);
        val1 = GET_T(tree);
        if (op == '+') {
            val += val1;
            tree.tokens[tree.free[0]].op = ADD;
            tree.free[0]++;
        }
        else if (op == '-') {
            val -= val1;
            tree.tokens[tree.free[0]].op = SUB;
            tree.free[0]++;
        }
        op = *s;
    }
    return val;
}

int GET_G(TREE_t tree) {

    int val = GET_E(tree);

    if (*s == '\0') {
        return val;
    }
    else {
        SYNTAX_ERROR();
    }
    return 0;
}

void SYNTAX_ERROR() {
    fprintf (stderr, "syntax error\n");
    abort();
}