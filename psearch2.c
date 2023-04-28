
/*
 * psearch1c.c
 *
 *  Created on: Oct 15, 2017
 *      Author: geek
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>






void psearch2(char *file_name_input, const char *character, FILE *fileWriter) {
    
    
        FILE *fileReader;
        ssize_t read;
        char *line = NULL;
        size_t len = 0;
        
    
        fileReader = fopen(file_name_input, "rb");
        if (fileReader
     == NULL) {
            printf("\n\nno input file\n\n");
            exit(EXIT_FAILURE);
        }
    
        int line_num = 1;
    
        while ((read = getline(&line, &len, fileReader)) != -1) {
    
            if (line[read - 1] ==
                '\n') 
                line[read - 1] = '\0';
    
            char *line_copy = strdup(line);
    
            char *tok = line, *end = line;
            while (tok != NULL) {
                strsep(&end, " ");
    
                if (strcmp(character, tok) == 0) {
                    fprintf(fileWriter
                    , "%s, %d: %s\n", file_name_input, line_num, line_copy);
                    break;
                }
    
                tok = end;
            }
    
            line_num++;
        }
    }
    int main(int argc, char **argv) {

    const char *const character = argv[1];
    const int input_count = atoi(argv[2]);
    const char *const file_name_output = argv[input_count + 3];

    FILE *fileWriter;
    fileWriter = fopen(file_name_output, "w+");

    int fd[2 * input_count];

    FILE *fpin[2 * input_count];
    FILE *fpout[2 * input_count];

    for (int i = 0; i < input_count; i++) 
        pipe(&fd[2*i]);     

    for (int i = 0; i < input_count; i++) {

        pid_t process_id_for_child = fork();

        if (process_id_for_child < 0) {
            printf("\nI cant create child proccess");
            exit(1);
        } else if (process_id_for_child == 0) {
            close(fd[2 * i]); 

            fpout[2 * i] = fdopen(fd[2 * i + 1], "w");
            psearch2(argv[i + 3], character, fpout[2 * i]);
            fclose(fpout[2 * i]);

            _exit(0);
        }

        else {
            close(fd[2 * i + 1]); 

            fpin[2 * i] = fdopen(fd[2 * i], "rb");

            char line[256];
            while (fgets(line, 256, fpin[2 * i]) != NULL)
                fprintf(fileWriter
                , "%s", line);

            fclose(fpin[2 * i]);

            close(fd[2 * i]); 

            wait(0); 
        }
    }


    return 0;
}
