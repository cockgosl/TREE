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
    UNDEF = 0,
    OP_T = 1,
    NUM_T = 2,
    VAR_T = 3,
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
    FBRACKET = '(',
    LBRACKET = ')',
};

union VALUE_t {
    double value;
    char name;
};

union DATA_t {
    OPERATION op_name;
    size_t number;
};

struct NODE_t {
    NODE_t* prev = NULL;
    NODE_t* left = NULL;
    NODE_t* right = NULL;
    TYPE type = UNDEF;
    DATA_t data = {};
    VALUE_t value = {};
    int number = -1;
};

struct TREE_t {
    NODE_t* root;
    double variables[100] = {};
    struct NODE_t* tokens = NULL;
    size_t* free = NULL;
};

NODE_t* NODE_READ (TREE_t* tree, char** current_pose, FILE* text);
void PRINTF_IN_DOT (NODE_t* node, FILE* output);
void PRINTG_NODE (NODE_t* node, FILE* output);
char* READ_BUFFER (FILE* text, size_t* counter);
void NODE_DELETE (NODE_t* node);
void PRINT_NODE (NODE_t* node, FILE* output);
NODE_t* COPY_NODE (NODE_t* node);
void COMPLETE_PRINTG(NODE_t* node, FILE* output);
void PRINTGB_NODE (FILE* output);
void PRINTGE_NODE (FILE* output);
void TREE_INIT (TREE_t* tree);
void TREE_DESTROY (TREE_t tree);
void TOKENS_C (FILE* input, TREE_t* tree);

#endif 