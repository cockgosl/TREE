#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <sys/stat.h>
#include <assert.h>

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

NODE_t* NODE_READ (char** current_pose, FILE* text);
char* READ_BUFFER(FILE* text);
void TREE_INIT (TREE_t* tree);
void LETS_PLAY (TREE_t* tree, NODE_t* node, size_t* sign);
void PRINT_NODE (NODE_t* node, FILE* output);
void PRINTG_NODE (NODE_t* node, FILE* output);
void NODE_CREATE (NODE_t* node);
void NODE_DELETE (NODE_t* node);
void PRINTF_IN_DOT (NODE_t* node, FILE* output);

int main() {
   return 0;
}

void TREE_INIT (TREE_t* tree) {
    tree->root = (NODE_t*) calloc (1 , sizeof(NODE_t));
    if (tree->root) {
        tree->root->yes = NULL;
        tree->root->no = NULL;
        strcpy (tree->root->question, "is it nothing?\n");
    }
    else {
        fprintf (stderr, "memory cannot be allocated\n");
        assert (tree->root);
    }  
}

void LETS_PLAY (TREE_t* tree, NODE_t* node, size_t* sign) {    
    char buffer[100] = {};

    for (; *sign != 1;) {

        for (char* temp = (char*) node->question; *temp != '\0'; temp++) {
            if (*temp != '\n') {
                printf ("%c", *temp);
            }
        }

        printf ("\n");
    
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            buffer[strcspn(buffer, "\n")] = '\0';
        }   

        if (strcmp (buffer, "no") == 0) {
            if (node->no == NULL) {
                tree->size += 1;
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
        node->no = (NODE_t*)calloc (1, sizeof (NODE_t));
        if (node->no) {
            node->no->prev = node;
            strcpy (node->no->question, node->question);
        }
        else {
            fprintf (stderr, "Memory cannot be allocated\n");
            assert(node->no);
        }

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
            assert (node->yes);
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
        for (size_t index = 0; index < 200 && (node->question)[index] != '\0' && (node->question)[index] != '\n'; index++) {
            if ((node->question)[index] != '\n') {
                fprintf (output, "%c", (node->question)[index]);
            }
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

void PRINTG_NODE (NODE_t* node, FILE* output) {
    if (node) {

        int temp = rand();

        PRINTF_IN_DOT (node, output);

        if (node->yes == NULL) {
            fprintf(output, "   \"%d\" [shape = doublecircle , color = \"black\", label=\" empty \"];\n", temp );
            fprintf(output, "   \"%p\"->\"%d\"\n", node, temp);
            temp = rand();
        }
        else if (node->yes->prev == node ) {
            PRINTF_IN_DOT (node->yes, output);
            fprintf(output, "   \"%p\"->\"%p\"\n", node, node->yes);
        }
        else {
            fprintf (output, "   \"%p\" [shape = record, color = \"red\", label=\" {addr = %p | data = POIZON  | left = POIZON | right = POIZON }\"];\n" ,node->yes, node->yes);
            fprintf(output, "   \"%p\"->\"%p\"\n", node, node->yes);
        }

        if (node->no == NULL) {
            fprintf(output, "   \"%d\" [shape = doublecircle , color = \"black\", label=\" empty \"];\n", temp );
            fprintf(output, "   \"%p\"->\"%d\"\n", node, temp);
            temp = rand();
        }
        else if (node->no->prev == node) { 
            PRINTF_IN_DOT (node->no, output);
            fprintf(output, "   \"%p\"->\"%p\"\n", node, node->no);
        }
        else {
            fprintf (output, "   \"%p\" [shape = record, color = \"red\", label=\" {addr = %p | data = POIZON  | left = POIZON | right = POIZON }\"];\n" ,node->no, node->no);
            fprintf(output, "   \"%p\"->\"%p\"\n", node, node->no);
        }

        if (node->yes) {
            PRINTG_NODE (node->yes, output);
        }
        if (node->no) {
            PRINTG_NODE (node->no, output);
        }
    }
    else {
        printf ("there is no such node\n");
    }
}

void PRINTF_IN_DOT (NODE_t* node, FILE* output) {

    fprintf(output, "   \"%p\" [shape = record, color = \"black\", label=\" {addr = %p |", node, node );

        for (size_t index = 0; index < 200 && (node->question)[index] != '\n' && (node->question)[index] != '\0'; index++) {
            if ((node->question)[index] != '\n') {
                fprintf (output, "%c", (node->question)[index]);
            }
        }

    fprintf (output, "| left = %p | right = %p }\"];\n" , node->yes, node->no);
}

NODE_t* NODE_READ (char** current_pose, FILE* text) {

    if (**current_pose == '(') {
        *current_pose += sizeof(char);

        if (**current_pose == '\"') {
            *current_pose += sizeof(char);

            NODE_t* node = (NODE_t*) calloc (1, sizeof (NODE_t));

            if (node) {

                sscanf(*current_pose, "%199[^\"]", node->question);

                *current_pose += strlen (node->question) + 1;

                node->yes = NODE_READ (current_pose, text);

                if (node->yes) {
                    node->yes->prev = node;
                }

                node->no = NODE_READ (current_pose, text);

                if (node->no) {
                    node->no->prev = node;
                }
            }
            else {
                fprintf (stderr, "memory cannot be allocated");
                assert(node);
            }

            return node;
        }
    }
    if (strncmp(*current_pose, "nil", strlen ("nil")) == 0) {
        *current_pose += strlen("nil") + 1;

        while (**current_pose == ')') {
            *current_pose += sizeof(char);
        }
        return NULL;
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
