#ifndef TREE_SHAPE 
#define TREE_SHAPE

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <cstring>
#include <sys/stat.h>
#include <ctype.h>
#include <float.h>
#include <math.h>

enum TYPE {
    OP_T = 0,
    NUM_T = 1,
    VAR_T = 2,
};

enum OPERATION {
    NONE = '0',
    ADD = '+',
    SUB = '-',
    MUL = '*',
    DIV = '/',
    SIN = 's',
    COS = 'c',
    TG = 't',
    LOG = 'l',
    POW = '^',
};

union VALUE_t {
    double value;
    char name;
};

union DATA_t {
    OPERATION op_name;
    size_t number;
};

struct TOKEN_t {
    int num = 0;
    OPERATION op = NONE;
};

struct NODE_t {
    NODE_t* prev = NULL;
    NODE_t* left = NULL;
    NODE_t* right = NULL;
    TYPE type = OP_T;
    DATA_t data = {};
    VALUE_t value = {};
};

struct TREE_t {
    NODE_t* root;
    double variables[100] = {};
    struct TOKEN_t* tokens = NULL;
    size_t* free = NULL;
};

NODE_t* NODE_READ (TREE_t* tree, char** current_pose, FILE* text);
void PRINTF_IN_DOT (NODE_t* node, FILE* output);
void PRINTG_NODE (NODE_t* node, FILE* output);
char* READ_BUFFER (FILE* text);
void NODE_DELETE (NODE_t* node);
void PRINT_NODE (NODE_t* node, FILE* output);
NODE_t* COPY_NODE (NODE_t* node);
void COMPLETE_PRINTG(NODE_t* node, FILE* output);
void PRINTGB_NODE (FILE* output);
void PRINTGE_NODE (FILE* output);
void TREE_INIT (TREE_t* tree);
void TREE_DESTROY (TREE_t tree);

#endif 