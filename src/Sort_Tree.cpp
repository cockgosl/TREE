#include <stdio.h>
#include <stdlib.h>

typedef int ELEM_t;

struct NODE_t {
    ELEM_t data;
    NODE_t* prev = NULL;
    NODE_t* left = NULL;
    NODE_t* right = NULL;
};

struct TREE_t {
    size_t size = 0;
    NODE_t* root = NULL;
};

void NODE_CREATE (TREE_t* tree, ELEM_t value);
void NODE_DELETE (NODE_t* node);
void PRINT_NODE (NODE_t* node);

void PRINTG_NODE (NODE_t* node, FILE* output);

void TREE_INIT (TREE_t* tree, ELEM_t value);

int main() {
    return 0;
}

void NODE_CREATE (TREE_t* tree, ELEM_t value) {
    if (tree) {
        NODE_t* temp = tree->root;
        for (size_t counter = 0; counter < tree->size; counter++) {
            if (temp->data < value) {
                if (temp->right == NULL) {
                    temp->right = (NODE_t*) calloc (1, sizeof (NODE_t));
                    if (temp->right) {
                        tree->size++;
                        temp->right->data = value;
                        temp->right->prev = temp;
                    }
                    else {
                        fprintf (stderr, "memory cannot be allocated\n");
                        assert (temp->right);
                    }
                    break;
                }
                temp = temp->right;
            }
            else {
                if (temp->left == NULL) {
                    temp->left = (NODE_t*) calloc (1, sizeof (NODE_t));
                    if (temp->left) {
                        tree->size++;
                        temp->left->data = value;
                        temp->left->prev = temp;
                    }
                    else {
                        fprintf (stderr, "memory cannot be allocated\n");
                        assert (temp->left);
                    }
                    break;
                }
                temp = temp->left;
            }
        }
    }
    else {
        printf ("there is no such tree\n");
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



void PRINT_NODE (NODE_t* node) {
    if (node) {
        printf ("(");
        printf ("%d", node->data);
        if (node->left) {
            PRINT_NODE(node->left);
        }
        if (node->right) {
            PRINT_NODE(node->right);
        }
        printf (")");
    }
    else {
        printf ("there is no such node\n");
    }
}

void PRINTG_NODE (NODE_t* node, FILE* output) {
    if (node) {
        int temp = rand();

        fprintf(output, "   \"%p\" [shape = record, color = \"black\", label=\" {addr = %p | data = %d  | left = %p | right = %p }\"];\n" ,node ,node ,node->data , node->left, node->right);

        if (node->left == NULL) {
            fprintf(output, "   \"%d\" [shape = doublecircle , color = \"black\", label=\" empty \"];\n", temp );
            fprintf(output, "   \"%p\"->\"%d\"\n", node, temp);
            temp = rand();
        }
        else if (node->left->prev == node ) {
            fprintf(output, "   \"%p\" [shape = record, color = \"black\", label=\" {addr = %p | data = %d  | left = %p | right = %p }\"];\n" ,node->left ,node->left ,node->left->data , node->left->left, node->left->right);
            fprintf(output, "   \"%p\"->\"%p\"\n", node, node->left);
        }
        else {
            fprintf (output, "   \"%p\" [shape = record, color = \"red\", label=\" {addr = %p | data = POIZON  | left = POIZON | right = POIZON }\"];\n" ,node->left, node->left);
            fprintf(output, "   \"%p\"->\"%p\"\n", node, node->left);
        }
        if (node->right == NULL) {
            fprintf(output, "   \"%d\" [shape = doublecircle , color = \"black\", label=\" empty \"];\n", temp);
            fprintf(output, "   \"%p\"->\"%d\"\n", node, temp);
            temp = rand();
        }
        else if (node->right->prev == node) { 
            fprintf(output, "   \"%p\" [shape = record, color = \"black\", label=\" {addr = %p | data = %d  | left = %p | right = %p }\"];\n" ,node->right ,node->right ,node->right->data , node->right->left, node->right->right);
            fprintf(output, "   \"%p\"->\"%p\"\n", node, node->right);
        }
        else {
            fprintf (output, "   \"%p\" [shape = record, color = \"red\", label=\" {addr = %p | data = POIZON  | left = POIZON | right = POIZON }\"];\n" ,node->right, node->right);
            fprintf(output, "   \"%p\"->\"%p\"\n", node, node->right);
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


void TREE_INIT (TREE_t* tree, ELEM_t value) {
    tree->size++;
    tree->root = (NODE_t*) calloc (1, sizeof (NODE_t));
    if (tree->root) {
        tree->root->data = value;
    }
    else {
        fprintf (stderr, "memory cannot be allocated\n");
        assert (tree->root);
    }
}



