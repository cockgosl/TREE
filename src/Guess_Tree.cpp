#include <stdio.h>
#include <stdlib.h>
#include <cstring>

struct NODE_t {
    char question[200] = {};
    NODE_t* yes = NULL;
    NODE_t* no = NULL;
    NODE_t* prev = NULL;
};


struct TREE_t {
    NODE_t* root = NULL;
    size_t size = 0;
};

void TREE_INIT (TREE_t* tree);
void LETS_PLAY (TREE_t* tree, NODE_t* node, size_t* sign);
void PRINT_NODE (NODE_t* node, FILE* output);
void PRINTG_NODE (NODE_t* node, FILE* output, int NODE_N);
void NODE_CREATE (NODE_t* node);
void NODE_DELETE (NODE_t* node);
void PRINTF_IN_DOT (NODE_t* node, FILE* output, int temp);

int main() {
    TREE_t tree1 = {};
    size_t sign = 0;
    TREE_INIT (&tree1);

    LETS_PLAY (&tree1,tree1.root, &sign);

    FILE* output = fopen ("g.gv", "wr");

    PRINTG_NODE(tree1.root , output, 0);

    fclose (output);

    FILE* output1 = fopen ("text.txt", "wr");

    PRINT_NODE (tree1.root, output1);

    fclose (output1);

    NODE_DELETE (tree1.root);
}

void TREE_INIT (TREE_t* tree) {
    tree->root = (NODE_t*) calloc (1 , sizeof(NODE_t));
    tree->root->yes = NULL;
    tree->root->no = NULL;
    strcpy (tree->root->question, "is it nothing?\n");  
}

void LETS_PLAY (TREE_t* tree, NODE_t* node, size_t* sign) {    

    char buffer[100] = {};


    for (; *sign != 1;) {

        for (char* temp = (char*) node->question; *temp != '\n'; temp++) {
            printf ("%c", *temp);
        }

        printf ("\n");
    
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            buffer[strcspn(buffer, "\n")] = '\0';
        }   

        if (strcmp (buffer, "no") == 0) {
            if (node->no == NULL) {
                NODE_CREATE (node);
                node = tree->root;
            }
            else {
                LETS_PLAY (tree, node->no, sign);
            }
        }
        else if (strcmp (buffer, "yes") == 0) {
            if (node->yes == NULL) {
                printf ("It was easy\n");
                *sign = 1;
            }
            else {
                LETS_PLAY (tree, node->yes, sign);
            }
        }
        else {
            printf ("play according to the rules\n");
        }
    }
}

void NODE_CREATE (NODE_t* node) {


    if (node) {


        node->no = (NODE_t*) calloc (1, sizeof (NODE_t));
        if (node->no) {
            node->no->prev = node;
        }
        else {
            fprintf (stderr, "Memory cannot be allocated\n");
        }

        strcpy (node->no->question, node->question);

        printf ("Then how would you differ it with question?\n");

        for (size_t ind = 0; ind < sizeof(node->question); ind++) {
            node->question[ind] = '\0';
        }

        if (fgets(node->question, sizeof(node->question), stdin) != NULL) {
            ;
        }
        else {
            printf ("input is wrong\n");
        }

        node->yes = (NODE_t*) calloc (1, sizeof (NODE_t));

        if (node->yes) {

            node->yes->prev = node;

            printf ("Well, what is the right answer?\n");


            if (fgets(node->yes->question, sizeof(node->yes->question), stdin) != NULL) {
                ;
            }   
        }
        else {
            fprintf (stderr, "memory cannot be allocated\n");
            abort();
        }
    }
    else {
        printf ("there is no such node\n");
    }
}

void NODE_DELETE (NODE_t* node) {
    if (node) {
        if (node->no) {
            NODE_DELETE (node->no);
            node->no = NULL;
        } 
        if (node->yes) {
            NODE_DELETE (node->yes);
            node->yes = NULL;
        }
        if (node->prev) {
            if (node->prev->no == node) {
                node->prev->no = NULL;
            }
            else if (node->prev->yes == node) {
                node->prev->yes = NULL;
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
        fprintf (output, "(\"");
        for (size_t index = 0; index < 200 && (node->question)[index] != '\n'; index++) {
            fprintf (output, "%c", (node->question)[index]);
        }
        if (node->yes) {
            fprintf (output, "\"");
            PRINT_NODE(node->yes, output);
        }
        else {
            fprintf (output, "\"(nil ");
        }
        if (node->no) {
            PRINT_NODE(node->no, output);
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

void PRINTG_NODE (NODE_t* node, FILE* output, int NODE_N) {
    if (node) {

        if (NODE_N == 0) {
            fprintf (output, "digraph G {\n");
        }

        int temp = NODE_N;
        int temp1 = rand();
        int temp2 = rand();

        PRINTF_IN_DOT (node, output, temp);

        if (node->yes == NULL) {
            fprintf(output, "   %d [shape = doublecircle , color = \"black\", label=\" empty \"];\n", temp1 );
            fprintf(output, "   %d->%d\n", temp, temp1);
        }
        else if (node->yes->prev == node ) {
            PRINTF_IN_DOT (node->yes, output, temp1);
            fprintf(output, "   %d->%d\n", temp, temp1);
        }
        else {
            fprintf (output, "   %d [shape = record, color = \"red\", label=\" {addr = %p | data = POIZON  | left = POIZON | right = POIZON }\"];\n" ,temp1, node->yes);
            fprintf(output, "   %d->%d\n", temp, temp1);
        }


        if (node->no == NULL) {
            fprintf(output, "   %d [shape = doublecircle , color = \"black\", label=\" empty \"];\n", temp2 );
            fprintf(output, "   %d->%d\n", temp, temp2);
        }
        else if (node->no->prev == node) { 
            PRINTF_IN_DOT (node->no, output, temp2);
            fprintf(output, "   %d->%d\n", temp, temp2);
        }
        else {
            fprintf (output, "   %d [shape = record, color = \"red\", label=\" {addr = %p | data = POIZON  | left = POIZON | right = POIZON }\"];\n" ,temp2, node->no);
            fprintf(output, "   %d->%d\n", temp, temp2);
        }

        if (node->yes) {
            PRINTG_NODE (node->yes, output, temp1);
        }
        if (node->no) {
            PRINTG_NODE (node->no, output, temp2);
        }

        if (NODE_N == 0) {
            fprintf (output, "}");
        }
    }
    else {
        printf ("there is no such node\n");
    }
}

void PRINTF_IN_DOT (NODE_t* node, FILE* output, int temp) {

    fprintf(output, "   %d [shape = record, color = \"black\", label=\" {addr = %p |", temp, node );

        for (size_t index = 0; index < 200 && (node->question)[index] != '\0'; index++) {
            if ((node->question)[index] != '\n') {
            fprintf (output, "%c", (node->question)[index]);
            }
        }

    fprintf (output, "| left = %p | right = %p }\"];\n" , node->yes, node->no);

}