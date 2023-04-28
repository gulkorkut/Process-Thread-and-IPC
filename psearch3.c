/*
 * psearch3.c
 *
 *  Created on: Oct 17, 2017
 *      Author: geek
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/*
Important note, the code should be compiled like this 

gcc -pthread -o psearch3 psearch3.c

otherwise, pthread library won't be recognized by the compiler 

and it will cause error. 

*/



typedef struct node {
    char *data;
    struct node *right;
} node;

void insert(node **tree, char *val) { 

    node *temp = NULL;
    if (!(*tree)) {
        temp = (node *) malloc(sizeof(node));
        temp->right = NULL;

	
        char *val_copy = strdup(val);
        temp->data = val_copy;
        *tree = temp;
        return;
    }


    insert(&(*tree)->right, val);
}

void inorder(node *tree, FILE* fp) {
    if (tree) {
        fprintf(fp, "%s\n", tree->data);
        inorder(tree->right, fp);
    }

}

typedef struct {

    char *nameOfTheInputFile;
    const char *keyword;
    node **list;
} readWriteToFilesArgs;

void *readWriteToFiles(void *args) {

    readWriteToFilesArgs *actual_args = args;

    FILE *fileReader;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fileReader = fopen(actual_args->nameOfTheInputFile, "r");

    if (fileReader == NULL) {
        printf("\n\nInput files don't exist. Exiting...\n\n");
        exit(EXIT_FAILURE);
    }


    int lineNumber = 1;

    while ((read = getline(&line, &len, fileReader)) != -1) {

        if (line[read - 1] ==
            '\n') 
            line[read - 1] = '\0';

        char *line_copy = strdup(line);

        char *tok = line, *end = line;
        while (tok != NULL) {
            strsep(&end, " ");

            if (strcmp(actual_args->keyword, tok) == 0) {

                char tmp[len];
                sprintf(tmp, "%s, %d: %s", actual_args->nameOfTheInputFile, lineNumber, line_copy);
                insert((actual_args->list), tmp);

                break;
            }

            tok = end;
        }

        lineNumber++;
    }

    return 0;
}


int main(int argc, char **argv) {
    
    
        const char *const keyword = argv[1];
    
        const int numberOfInputs = atoi(argv[2]);
    
        node *root[numberOfInputs];
        readWriteToFilesArgs *pfa[numberOfInputs];
    
        for (int i = 0; i < numberOfInputs; i++) {
            root[i] = NULL;
            pfa[i] = malloc(sizeof *pfa[0]);
        }
    
        int err;
        pthread_t tid[numberOfInputs];
    
        for (int i = 0; i < numberOfInputs; i++) {
            pfa[i]->nameOfTheInputFile = argv[i + 3];
            pfa[i]->keyword = keyword;
            pfa[i]->list = &root[i];
    
            err = pthread_create(&(tid[i]), NULL, &readWriteToFiles, pfa[i]);
    
            if (err != 0)
                printf("\ncan't create thread :[%s]", strerror(err));
    
    
            pthread_join(tid[i], NULL);
    
    
        }
    
    
        // combining .txt files.

        FILE *fileWriter;
    
        fileWriter = fopen(argv[numberOfInputs + 3], "w+");
    
        
        for (int i = 0; i < numberOfInputs; i++) 
            inorder(root[i], fileWriter);
        
    
        fclose(fileWriter);
    
        for (int i = 0; i < numberOfInputs; i++) {
            free(root[i]);
            free(pfa[i]);
        }
    }
    
