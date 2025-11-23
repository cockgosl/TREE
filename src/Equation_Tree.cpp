#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <cstring>
#include <sys/stat.h>
#include <ctype.h>
#include <math.h>
#include <float.h>

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
NODE_t* TREE_DIFFERENTIATE (TREE_t tree, NODE_t* node, char variable);
void NODE_DELETE (NODE_t* node);
void PRINT_NODE (NODE_t* node, FILE* output);
NODE_t* COPY_NODE (NODE_t* node);

int main () {
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
            if (tree->free != 0) {
                for (size_t ind = 0; ind <= tree->free; ind++) {
                    if (fabs (tree->variables[ind] - *buffer) < DBL_EPSILON) {
                        node->data.number = ind;
                        break;
                    }
                    if (ind == tree->free) {
                        node->data.number = tree->free;
                        tree->variables[tree->free] = (double)*buffer;
                        tree->free++;
                        break;
                    }
                }   
            }
            else {
                node->data.number = tree->free;
                tree->variables[tree->free] = (*buffer);
                tree->free++;
            }
            node->value.name = *buffer;
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
    assert (node);
    assert (output);

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
                break;

            case (ADD):
                new_node->type = OP_T;
                new_node->data.op_name = ADD;
                new_node->left = TREE_DIFFERENTIATE (tree, node->left, variable);
                new_node->right = TREE_DIFFERENTIATE (tree, node->right, variable);
                break;
            case (SUB):
                new_node->type = OP_T;
                new_node->data.op_name = SUB;
                new_node->left = TREE_DIFFERENTIATE (tree, node->left, variable);
                new_node->right = TREE_DIFFERENTIATE (tree, node->right, variable);
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

NODE_t* COPY_NODE (NODE_t* node) {
    assert(node);
    NODE_t* node_c = (NODE_t*)calloc (1, sizeof(NODE_t));\
    assert(node_c);

    node_c->type = node->type;
    if (node->type == OP_T) {
        node_c->data.op_name = node->data.op_name;
    }  
    else if (node->type == NUM_T) {
        node_c->value.value = node->value.value;
    }
    else {
        node_c->data.number = node->data.number;
        node_c->value.name = node->value.name;
    }

    if (node->left) {
        node_c->left = COPY_NODE (node->left);
        node_c->left->prev = node_c;
    }
    if (node->right) {
        node_c->right = COPY_NODE (node->right);
        node_c->right->prev = node_c;
    }
    return (node_c);
}


