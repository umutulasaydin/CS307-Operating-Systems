#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>


int
main(int argc, char *argv[])
{
    FILE * file = fopen("command.txt", "r");
    char text[1000];
    
    char *fc[3];
    char *sc[6];
    char temp[50];
    char c1[500];
    char c2[500];
    int second = 0;
    int fic = 0;
    int sec = 0;
    while(fscanf(file,"%s",temp) == 1)
    {
        strcat(text,temp);
        strcat(text, " ");

        if (strcmp(temp, "|") == 0){
            
            second = 1;
            continue;
        }
        
        if (second == 0){
           
            if (fic == 0){
                fc[0] = strdup(temp);
                fic = 1;
            }
            
            else{
                
                strcat(c1, temp);
                strcat(c1, " ");
                
            }
            

        }
        
        else{
            if(sec==0){
                
                sec = 1;
            }
            else{
                strcat(c2, temp);
                strcat(c2, " ");
            }
        }
        
        
    }
    c1[strlen(c1)-1] = '\0';
    c2[strlen(c2)-1] = '\0';
    fc[1] = strdup(c1);
    fc[2] = NULL;
    


    sc[5] = NULL;
    sc[4] = strdup("./temp.txt");
    sc[0] = strdup("grep");
    sc[1] = strdup("-e");
    sc[2] = strdup("-c");
    sc[3] = strdup("-A 2");






    printf("I'm SHELL process, with PID: %d - Main command is: %s\n", (int) getpid(), text);



 
    
    int rc = fork();

    if (rc < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }

    //First child
    else if (rc == 0){
        printf("I'm MAN process, with PID: %d - My command is: %s %s\n", (int) getpid(), fc[0], fc[1]);


        close(STDOUT_FILENO);
        open("./temp.txt", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);

        execvp(fc[0], fc);
        

        

        
    }
    
    else{
        
        wait(NULL);

        
        int rc2 = fork();

        if (rc2 < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
        }

        //Second child
        else if (rc2 == 0){
            printf("I'm GREP process, with PID: %d - My command is: %s %s\n", (int) getpid(), sc[0],c2);

            close(STDOUT_FILENO);
        	open("./output.txt", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);

            
    		
    		execvp(sc[0], sc);

            
        }

        //Parent
        else{
            
            wait(NULL);
            printf("I'm SHELL process, with PID: %d - execution is completed, you can find the results in output.txt\n", (int) getpid());
        }
        
    }
        
    return 0;
}