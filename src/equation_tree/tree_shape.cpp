#include "tree_shape.h"

void TREE_INIT (TREE_t* tree) {
    tree->free = (size_t*)calloc(1, sizeof(size_t));
    tree->tokens = (NODE_t*)calloc (200, sizeof(NODE_t));
}


void TREE_DESTROY (TREE_t tree) {
    free(tree.free);
    free(tree.tokens);
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
                for (size_t ind = 0; ind <= tree->free[0]; ind++) {
                    if (fabs (tree->variables[ind] - *buffer) < DBL_EPSILON) {
                        node->data.number = ind;
                        break;
                    }
                    if (ind == tree->free[0]) {
                        node->data.number = tree->free[0];
                        tree->variables[tree->free[0]] = (double)*buffer;
                        tree->free++;
                        break;
                    }
                }   
            }
            else {
                node->data.number = tree->free[0];
                tree->variables[tree->free[0]] = (*buffer);
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


        node->right = NODE_READ (tree, current_pose, text);

        return node;
    }
    return NULL;
}

char* READ_BUFFER (FILE* text, size_t* size) {
    assert (text);

    struct stat statistic = {};

    char* buffer = NULL;
    int descriptor = fileno(text); 
    fstat (descriptor, &statistic);    

    *size = statistic.st_size;

    buffer = (char*) calloc (sizeof (char), *size + sizeof(char));
    if (buffer) {
        buffer[*size] = '\0';
        fread (buffer, sizeof(char), *size, text);
    }
    else {
        fprintf (stderr, "memory cannot be allocated");
        return 0;
    }
    
    return buffer;   
}

void PRINTF_IN_DOT (NODE_t* node, FILE* output) {
    if (node->type == 1) {
            fprintf (output, "   \"%p\" [shape = record, color = \"black\", label=\" {addr = %p |type = OP_T |data = %c |left = %p | right = %p }\"];\n", node, node, node->data.op_name , node->left, node->right);
        }
    else if (node->type == 2) {
        fprintf (output, "   \"%p\" [shape = record, color = \"black\", label=\" {addr = %p |type = NUM_T |data = %lf |left = %p | right = %p }\"];\n", node, node, node->value.value , node->left, node->right);
    }
    else {
        fprintf (output, "   \"%p\" [shape = record, color = \"black\", label=\" {addr = %p |type = VAR_T |data = %ld(%c) |left = %p | right = %p }\"];\n", node, node, node->data.number, node->value.name , node->left, node->right);
    }
}

void PRINTGB_NODE (FILE* output) {
    fprintf (output, "digraph G {\n");
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
        else {
            PRINTF_IN_DOT (node->left, output);
            fprintf (output, "   \"%p\"->\"%p\"\n", node, node->left);
        }

        if (node->right == NULL) {
            fprintf (output, "   \"%d\" [shape = doublecircle , color = \"black\", label=\" empty \"];\n", temp );
            fprintf (output, "   \"%p\"->\"%d\"\n", node, temp);
            temp = rand();
        }
        else { 
            PRINTF_IN_DOT (node->right, output);
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

void COMPLETE_PRINTG(NODE_t* node, FILE* output) {
    PRINTGB_NODE(output);
    PRINTG_NODE (node, output);
    PRINTGE_NODE(output);
}

void PRINTGE_NODE (FILE* output) {
    fprintf (output, "}");
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
        if (node->type == OP_T) {
            fprintf (output, "%c", node->data.op_name);
        }
        else if (node->type == NUM_T) {
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

