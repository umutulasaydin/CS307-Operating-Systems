#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    FILE *file = fopen("commands.txt", "r");
    FILE *parse = fopen("parse.txt", "w");
    char temp[128];
    char *token;
    
    
    

    while (fgets(temp, sizeof(temp), file) != NULL)
    {
        int redirection_sym = 0;
        int background = 0;
        int options = 0;
        int inputs = 0;
        char *cd[6];
        char redirection[5];
        char filename[30] = "./";
        int count = 0;
        fprintf(parse, "----------\n");
        token = strtok(temp, " ");
        while (token != NULL)
        {
            token[strcspn(token, "\n")] = '\0';
            token[strcspn(token, "\r")] = '\0';
            
            
            if (count == 0)
            {
                cd[count] = strdup(token);
                fprintf(parse, "Command: %s\n", token);
                count++;
                
            }

            else if (count == 1 && token[0] != '-')
            {
                cd[count] = strdup(token);
                fprintf(parse, "Inputs: %s\n", token);
                inputs = 1;
                count++;
            }

            else if (token[0] == '-')
            {
                if (inputs == 0)
                {
                    fprintf(parse, "Inputs:\n");
                    inputs = 1;
                }
                cd[count] = strdup(token);
                fprintf(parse, "Options: %s\n", token);
                options = 1;
                count++;
            }
            
            else if (token[0] == '<' || token[0] == '>')
            {
                if (options == 0)
                {
                    fprintf(parse, "Options:\n");
                    options = 1;
                }
                strcat(redirection, token);
                fprintf(parse, "Redirection: %s\n", token);
                redirection_sym = 1;
            }

            else if (redirection_sym == 1 && token[0] != '&')
            {
                
                strcat(filename, token);
            }

            else if (token[0] == '&')
            {
                
                if (inputs == 0)
                {
                    fprintf(parse, "Inputs:\n");
                    inputs = 1;
                }
                if (options == 0)
                {
                    fprintf(parse, "Options:\n");
                    options = 1;
                }
                if (redirection_sym == 0)
                {
                    fprintf(parse, "Redirection: -\n");
                    redirection_sym = 1;
                }
                cd[count] = strdup(token);
                fprintf(parse, "Background job: y\n");
                background = 1;
                count++;
            }

            token = strtok(NULL, " ");
            
        }
        if (inputs == 0)
        {
            fprintf(parse, "Inputs:\n");
            inputs = 1;
        }
        if (options == 0)
        {
            fprintf(parse, "Options:\n");
            options = 1;
        }
        if (redirection_sym == 0)
        {
            fprintf(parse, "Redirection: -\n");
            redirection_sym = 1;
        }
        if (background == 0)
        {
            fprintf(parse, "Background job: n\n");
            background = 1;
        }
        cd[count+1] = NULL;
        fprintf(parse, "----------\n");
        
   
        
        
        
        
        int rc = fork();



    
        if (rc == 0){
            printf("%s\n", cd[0]);

            if (redirection[0] == '>'){
                printf("%d", count);
                close(STDOUT_FILENO);
                open(filename, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
                int i = 0;
                for(;i<count;i++){
                    printf("%s\n", cd[i]);
                }
                //execvp(cd[0], cd);
            }

        
/*
            else if (redirection[0] == '<'){
                //close(STDIN_FILENO);
                //open(filename, O)
            }
            
*/            
        }
        
        
        
       filename[0] = 0;
       redirection[0] = 0;
    }

    return 0;
}