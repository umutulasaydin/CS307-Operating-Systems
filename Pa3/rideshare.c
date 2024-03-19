#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//Booleans
int waiting = 0;
int deploy = 1;
int wait_a = 0;
int wait_b = 0;

//Integers
int waiting_a = 0;
int waiting_b = 0;
int total_waiting = 0;
int deploy_a = 0;
int deploy_b = 0;
int count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *fan(void *arg)
{
    char* team_name = (char*) arg;
    

    pthread_mutex_lock(&mutex);

    while(waiting){
        pthread_cond_wait(&cond, &mutex);
    }

    printf("Thread ID: %ld, Team: %s, I am looking for a car.\n", pthread_self(), team_name);

    if(*team_name == 'A'){
        waiting_a++;
    }
    else{
        waiting_b++;
    }
    total_waiting++;

    if (deploy == 1 && ((waiting_a>=2 && waiting_b>=2) || total_waiting>=4)){
        if ((waiting_a!=1 && waiting_b!=1) || waiting_a == 4 || waiting_b == 4){
            deploy = 0;
            waiting = 1;
            
            if (waiting_a == 1){
                wait_a = 1;
            }
            else if (waiting_b == 1){
                wait_b = 1;
            }
        }
    }

    while (deploy){
        pthread_cond_wait(&cond, &mutex);
    }

    while(*team_name == 'A' && wait_a){
        pthread_cond_wait(&cond, &mutex);
    }

    while(*team_name == 'B' && wait_b){
        pthread_cond_wait(&cond, &mutex);
    }

    while(deploy){
        pthread_cond_wait(&cond, &mutex);
    }
    
    if (*team_name == 'A'){
        waiting_a--;
        deploy_a++;
    }

    else{
        waiting_b--;
        deploy_b++;
    }

    total_waiting--;
    count++;
    
    if (count == 3){
        if(deploy_a ==2 && deploy_b == 1){
            wait_a = 1;
        }
        else if(deploy_a ==1 && deploy_b ==2){
            wait_b = 1;
        }
    }
    pthread_cond_broadcast(&cond);

    printf("Thread ID: %ld, Team: %s, I found a spot in the car.\n", pthread_self(), team_name);

    if (count == 4){
        printf("Thread ID: %ld, Team: %s, I am the captain and driving the car.\n", pthread_self(), team_name);
        deploy = 1;
        waiting = 0;
        count = 0;
        wait_a = 0;
        wait_b = 0;
        deploy_a = 0;
        deploy_b = 0;
    }

    pthread_mutex_unlock(&mutex);

    return NULL;
}


int main(int argc, char *argv[])
{
    if (argc == 3){
        
        int teamA = atoi(argv[1]);
        int teamB = atoi(argv[2]);

        char *a = malloc(sizeof(char));
        char *b = malloc(sizeof(char));
        *a = 'A';
        *b = 'B';

        if (teamA % 2 == 0 && teamB % 2 == 0 && (teamA+teamB) % 4 == 0){

            pthread_t threads[teamA+teamB];

            for (int i = 0; i < teamA; i++){
                pthread_create(&threads[i], NULL, fan, a);
            }

            for (int i = teamA; i < teamA+teamB; i++){
                pthread_create(&threads[i], NULL, fan, b);
            }

            for (int i = 0; i < teamA+teamB; i++){
                pthread_join(threads[i], NULL);
            }
        }
    }

    printf("The main terminates\n");
    return 0;
}