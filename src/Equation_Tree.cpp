#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <cstring>
#include <sys/stat.h>
#include <ctype.h>
#include <math.h>

enum TYPE {
    OP_T = 0,
    NUM_T = 1,
    VAR_T = 2,
};

enum OPERATION {
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
    size_t free = 0;
};

NODE_t* NODE_READ (TREE_t* tree, char** current_pose, FILE* text);
void PRINTF_IN_DOT (NODE_t* node, FILE* output);
void PRINTG_NODE (NODE_t* node, FILE* output);
char* READ_BUFFER (FILE* text);
double TREE_SOLVE (TREE_t* tree, NODE_t* node);
void NODE_DELETE (NODE_t* node);
void PRINT_NODE (NODE_t* node, FILE* output);

int main () {
    TREE_t tree1 = {};
    double solution1 = NAN;

    FILE* input = fopen ("dump_inf/equation.txt", "r");

    char* current_pose = READ_BUFFER(input);

    char* temp = current_pose;

    tree1.root = NODE_READ (&tree1, &current_pose, input);

    tree1.variables[0] = 10;
    tree1.variables[1] = 5;

    solution1 = TREE_SOLVE (&tree1, tree1.root);

    printf ("SOLUTION: %lf\n", solution1);

    free (temp);

    fclose (input);

    FILE* output = fopen ("dump_inf/g.gv", "wr");

    PRINTG_NODE(tree1.root , output);

    fclose (output);

    FILE* output1 = fopen ("dump_inf/text.txt", "wr");

    PRINT_NODE (tree1.root, output1);

    fclose (output1);

    NODE_DELETE (tree1.root);
    return 0;
}

NODE_t* NODE_READ (TREE_t* tree, char** current_pose, FILE* text) {

    if (strncmp(*current_pose + 1, "nil", strlen ("nil")) == 0) {
        *current_pose += strlen("nil") + 1;

        while (**current_pose == ')') {
            *current_pose += sizeof(char);
        }

        return NULL;
    }
    if (**current_pose == '(') {
        char buffer[10] = {};
        char temp[] = "+-*/sctl^";

        *current_pose += sizeof(char);

        NODE_t* node = (NODE_t*) calloc (1, sizeof (NODE_t));
        assert (node);
        
        sscanf(*current_pose, "%9[^(]", buffer);

        if (isdigit(*buffer)) {
            node->type = NUM_T;
            sscanf (buffer, "%lf", &(node->value.value)); 
        }
        else if (isalpha(*buffer) && strlen(buffer) == 1) {
            node->type = VAR_T;
            node->data.number = tree->free;
            node->value.name = *buffer;
            tree->free++;
        }
        else {
            for (size_t ind = 0; ind < strlen(temp); ind++) {
                if (**current_pose == temp[ind]) {
                    node->type = OP_T;
                    node->data.op_name = (OPERATION)*buffer;
                    break;
                }
            }
        }

        *current_pose += strlen (buffer);

        node->left = NODE_READ (tree, current_pose, text);

        if (node->left) {
            node->left->prev = node;
        }

        node->right = NODE_READ (tree, current_pose, text);

        if (node->right) {
            node->right->prev = node;
        }

        return node;
    }
    return NULL;
}

char* READ_BUFFER (FILE* text) {
    assert (text);

    size_t size = 0;

    struct stat statistic = {};

    char* buffer = NULL;
    int descriptor = fileno(text); 
    fstat (descriptor, &statistic);    

    size = statistic.st_size;

    buffer = (char*) calloc (sizeof (char), size + sizeof(char));
    if (buffer) {
        buffer[size] = '\0';
        fread (buffer, sizeof(char), size, text);
    }
    else {
        fprintf (stderr, "memory cannot be allocated");
        assert(buffer);
    }
    
    return buffer;   
}

void PRINTF_IN_DOT (NODE_t* node, FILE* output) {
    if (node->type == 0) {
            fprintf (output, "   \"%p\" [shape = record, color = \"black\", label=\" {addr = %p |type = OP_T |data = %c |left = %p | right = %p }\"];\n", node, node, node->data.op_name , node->left, node->right);
        }
    else if (node->type == 1) {
        fprintf (output, "   \"%p\" [shape = record, color = \"black\", label=\" {addr = %p |type = NUM_T |data = %lf |left = %p | right = %p }\"];\n", node, node, node->value.value , node->left, node->right);
    }
    else {
        fprintf (output, "   \"%p\" [shape = record, color = \"black\", label=\" {addr = %p |type = VAR_T |data = %ld(%c) |left = %p | right = %p }\"];\n", node, node, node->data.number, node->value.name , node->left, node->right);
    }
}

void PRINTG_NODE (NODE_t* node, FILE* output) {
    if (node) {

        int temp = rand();

        PRINTF_IN_DOT (node, output);

        if (node->left == NULL) {
            fprintf (output, "   \"%d\" [shape = doublecircle , color = \"black\", label=\" empty \"];\n", temp );
            fprintf (output, "   \"%p\"->\"%d\"\n", node, temp);
            temp = rand();
        }
        else if (node->left->prev == node ) {
            PRINTF_IN_DOT (node->left, output);
            fprintf (output, "   \"%p\"->\"%p\"\n", node, node->left);
        }
        else {
            fprintf (output, "   \"%p\" [shape = record, color = \"red\", label=\" {addr = %p | data = POIZON  | left = POIZON | right = POIZON }\"];\n" ,node->left, node->left);
            fprintf (output, "   \"%p\"->\"%p\"\n", node, node->left);
        }

        if (node->right == NULL) {
            fprintf (output, "   \"%d\" [shape = doublecircle , color = \"black\", label=\" empty \"];\n", temp );
            fprintf (output, "   \"%p\"->\"%d\"\n", node, temp);
            temp = rand();
        }
        else if (node->right->prev == node) { 
            PRINTF_IN_DOT (node->right, output);
            fprintf (output, "   \"%p\"->\"%p\"\n", node, node->right);
        }
        else {
            fprintf (output, "   \"%p\" [shape = record, color = \"red\", label=\" {addr = %p | data = POIZON  | left = POIZON | right = POIZON }\"];\n" ,node->right, node->right);
            fprintf (output, "   \"%p\"->\"%p\"\n", node, node->right);
        }

        if (node->left) {
            PRINTG_NODE (node->left, output);
        }
        if (node->right) {
            PRINTG_NODE (node->right, output);
        }
    }
    else {
        printf ("there is no such node\n");
    }
}

void NODE_DELETE (NODE_t* node) {
    if (node) {
        if (node->right) {
            NODE_DELETE (node->right);
            node->right = NULL;
        } 
        if (node->left) {
            NODE_DELETE (node->left);
            node->left = NULL;
        }
        if (node->prev) {
            if (node->prev->right == node) {
                node->prev->right = NULL;
            }
            else if (node->prev->left == node) {
                node->prev->left = NULL;
            }
            else {
                fprintf (stderr, "the tie is wrong\n");
                abort();
            }
        }
        free (node);
    }
    else {
        fprintf (stderr, "the given node dosn't exist\n");
    }
}

void PRINT_NODE (NODE_t* node, FILE* output) {
    if (node) {
        fprintf (output, "(");
        if (node->type == 0) {
            fprintf (output, "%c", node->data.op_name);
        }
        else if (node->type == 1) {
            fprintf (output, "%lf", node->value.value);
        }
        else {
            fprintf (output, "%c", node->value.name);
        }
        if (node->left) {
            PRINT_NODE(node->left, output);
        }
        else {
            fprintf (output, "(nil ");
        }
        if (node->right) {
            PRINT_NODE(node->right, output);
        }
        else {
            fprintf (output, "nil)");
        }
        fprintf (output, ")");
    }
    else {
        fprintf (output, "there is no such node\n");
    }
}

double TREE_SOLVE (TREE_t* tree, NODE_t* node) {
    double solution = NAN;
    if (node->type == 0) {
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
    /*ADD = '+',
    SUB = '-',
    MUL = '*',
    DIV = '/',
    SIN = 's',
    COS = 'c',
    TG = 't',
    LOG = 'l',
    POW = '^',*/
        }
    }
    else if (node->type == 1) {
        solution = node->value.value;
    }
    else {
        solution = tree->variables[node->data.number];
    }
    return solution;
}

