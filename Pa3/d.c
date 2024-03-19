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
   
	int i;
    for (i=0; i < 1000; i++){
int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
		

		// now exec "wc"...
        char *myargs[3];
        myargs[0] = strdup("./deneme");	
        myargs[1] = strdup("2"); 	
	    myargs[2] = strdup("4");
        myargs[3] = NULL;           	
        execvp(myargs[0], myargs);  	
    } else {
     
        wait(NULL);
    }
    }
    
    return 0;
}
