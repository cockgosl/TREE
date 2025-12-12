#include <stdio.h>
#include <stdlib.h>

#include "my_math.h"
#include "tree_shape.h"

void SYNTAX_ERROR();
NODE_t* GET_N(NODE_t* node, char** s);
NODE_t* GET_P(NODE_t* node, char** s);
NODE_t* GET_T(NODE_t* node, char** s);
NODE_t* GET_E(NODE_t* node, char** s);
NODE_t* GET_G(NODE_t* node, char** s);


int main() {
    char* str = NULL;
    TREE_t* tree;
    str = (char*)calloc(100, sizeof(char));
    char* temp = str;
    scanf("%s", str);
    tree->root = GET_G(&str);
    free(temp);
}

NODE_t* GET_N (NODE_t* node, char** s) {
    NODE_t* val = NULL;
    size_t sign = 0;
    while ('0' <= **s && **s<= '9') {
        val = val*10 + **s - '0';
        sign++;
        *s += sizeof(char);
    }
    if (sign == 0) {
        SYNTAX_ERROR();
    }
    return val;
}

NODE_t* GET_P (NODE_t* node, char** s) {
    int val = 0;
    if (**s == '(') {
        *s += sizeof(char);
        val = GET_E(NODE_t* node, s);
        if (**s == ')') {
            *s += sizeof(char);
        }
        else {
            SYNTAX_ERROR();
        } 
    }
    else {
        val = GET_N(NODE_t* node, s);
    }
    return val;
}

NODE_t* GET_T(NODE_t* node, char** s) {
    int val = GET_P(NODE_t* node, s);
    int val1 = 0;
    char op = **s;
    while (**s == '*' || **s == '/') {
        *s += sizeof(char);
        val1 = GET_P(NODE_t* node, s);
        if (op == '*') {
        val = val * val1;
        }
        else if (op == '/') {
            val = val / val1;
        }
        op = **s;
    }
    return val;
}

NODE_t* GET_E(NODE_t* node, char** s) {
    int val = GET_T(NODE_t* node, s);
    int val1 = 0;
    char op = **s;
    while (**s == '+' || **s == '-') {
        *s += sizeof(char);
        val1 = GET_T(NODE_t* node, s);
        if (op == '+') {
        val = val + val1;
        }
        else if (op == '-') {
            val = val - val1;
        }
        op = **s;
    }
    return val;
}

NODE_t* GET_G(NODE_t* node, char** s) {
    int val = GET_E(NODE_t* node, s);
    if (**s == '\0') {
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