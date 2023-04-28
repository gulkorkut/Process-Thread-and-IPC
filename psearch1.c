

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


void psearch1(char *file_name_input, char *file_name_output, const char *keyword) {
    
        FILE *fileReader, *fileWriter;
        char *line = NULL;
        ssize_t read;
        size_t length = 0;
        int line_num = 1;
        
        fileWriter = fopen(file_name_output, "w+");
        fileReader = fopen(file_name_input, "rb");
        
    
        if (fileReader == NULL){
            printf("File reader equals null \n");
            exit(EXIT_FAILURE);
        }
            
    
        
    
        while ((read = getline(&line, &length, fileReader)) != -1) {
    
            if (line[read - 1] ==
                '\n') 
                line[read - 1] = '\0';
    
            char *line_copy = strdup(line);
    
            char *tok = line, *end = line;
            while (tok != NULL) {
                strsep(&end, " ");
    
                if (strcmp(keyword, tok) == 0) {
                    fprintf(fileWriter, "%s, %d: %s\n",file_name_input, line_num, line_copy);
                    break;
                }
    
                tok = end;
            }
    
            line_num++;
        }
    
        fclose(fileWriter);
    
    }
    int main (int argc, char *argv[]){

const char *const keyword=argv[1];
const int input_count = atoi(argv[2]);
    for (int i =0; i < input_count; i++) { 

        pid_t process_id_for_child = fork();

        if (process_id_for_child < 0) {
            printf("\nI cant create child process");
           exit(1);
        } else if (process_id_for_child == 0) {
            char str[50];
            sprintf(str, "%d.txt", i);
            psearch1(argv[i+3], str, keyword);
            _exit(0);
        } else
		wait(0);	

    }


    FILE *fileReader, *fileWriter;
    char *line = NULL;
    size_t length = 0;
    ssize_t read;

    fileWriter = fopen(argv[input_count+3], "w+"); 

    for (int i = 0; i < input_count; i++) {
        char str[10];
        sprintf(str, "%d.txt", i);

        fileReader = fopen(str, "rb");

        if (fileReader == NULL)
            exit(EXIT_FAILURE);            
           
        while ((read = getline(&line, &length, fileReader)) != -1) {

            fprintf(fileWriter, "%s", line);

        }

        remove(str);
    }

    fclose(fileWriter);
    fclose(fileReader);


    return 0;

}

    

