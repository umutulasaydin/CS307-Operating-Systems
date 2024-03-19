#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_STUDENTS 100
#define MAX_ASSISTANTS 100

int num_students, num_assistants;
int students_in_room = 0, assistants_in_room = 0;
int students_ready = 0;


sem_t students_enter;
sem_t assistants_enter;
sem_t demo_session;
sem_t students_leave;
sem_t students_participate;


void* student_thread() {
    
    
    printf("Thread ID: %d, Role: Student, I want to enter the classroom.\n", pthread_self());
    while (students_in_room+1 > 3*assistants_in_room){}
    printf("Thread ID: %d, Role: Student, I entered the classroom.\n", pthread_self());
    students_in_room++;
    
    
    sem_wait(&students_enter);
    printf("Thread ID: %d, Role: Student, I am now participating.\n", pthread_self());
    students_ready++;
    sem_post(&students_participate);


    sem_wait(&students_leave);
    printf("Thread ID: %d, Role: Student, I am leaving.\n", pthread_self());
    students_ready--;
    students_in_room--;    

    pthread_exit(NULL);
}

void* assistant_thread() {

    
    printf("Thread ID: %d, Role: Assistant, I entered the classroom.\n", pthread_self());
    assistants_in_room++;
 
    
    sem_wait(&assistants_enter);
    
    while (students_in_room < 2) {} 
    sem_wait(&demo_session);
    printf("Thread ID: %d, Role: Assistant, I am now participating.\n", pthread_self());
    sem_post(&students_enter);
    sem_post(&students_enter);
    sem_wait(&students_participate);
    sem_wait(&students_participate);

    printf("Thread ID: %d, Role: Assistant, Demo is over.\n", pthread_self());
    sem_post(&students_leave);
    sem_post(&students_leave);
    sem_post(&demo_session);
    sem_post(&assistants_enter);
   
    while(3*assistants_in_room -3 < students_in_room ){}
    printf("Thread ID: %d, Role: Assistant, I left the classroom.\n", pthread_self());
    assistants_in_room--;
   
    
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Invalid number of arguments.\n");
        printf("Usage: %s <num_assistants> <num_students>\n", argv[0]);
        return 1;
    }
    
    num_assistants = atoi(argv[1]);
    num_students = atoi(argv[2]);
    
    if (num_assistants <= 0 || num_students != 2 * num_assistants) {
        printf("Invalid number of participants.\n");
        return 1;
    }
    
    pthread_t students[MAX_STUDENTS];
    pthread_t assistants[MAX_ASSISTANTS];

    
 
    sem_init(&students_enter, 0, 0);
    sem_init(&assistants_enter, 0, 1);
    sem_init(&demo_session, 0, 1);
    sem_init(&students_leave, 0, 0);
    sem_init(&students_participate, 0, 0);
    
    for (int i = 0; i < num_students; i++) {
    
        pthread_create(&students[i], NULL, student_thread, NULL);
    }
    
    for (int i = 0; i < num_assistants; i++) {
        pthread_create(&assistants[i], NULL, assistant_thread, NULL);
    }
    
    for (int i = 0; i < num_students; i++) {
        pthread_join(students[i], NULL);
    }
    
    for (int i = 0; i < num_assistants; i++) {
        pthread_join(assistants[i], NULL);
    }
    

    sem_destroy(&students_enter);
    sem_destroy(&assistants_enter);
    sem_destroy(&demo_session);
    sem_destroy(&students_leave);
    sem_destroy(&students_participate);

    
    printf("The main thread terminates.\n");
    
    return 0;
}
