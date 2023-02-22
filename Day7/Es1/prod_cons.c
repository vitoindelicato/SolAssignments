#include <Pthreads.h>
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;  /* inizializzo la lock */

/* inizializzo le condition variables */
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;


void* producer(void* args){
    uint seed = time(NULL);
    int* buffer = args;
    int i = 0;
    while(i < 20){
        Pthread_mutex_lock(&mtx);

        while(buffer[0] > -1){
            Pthread_cond_wait(&empty, &mtx);
        }

        buffer[0] = (rand_r(&seed)) %100 + 1;
        i++;

        Pthread_mutex_unlock(&mtx);
        Pthread_cond_signal(&full);
    }
    return NULL;
}



void* consumer(void* args){
    int* buffer = args;
    int i = 0;
    while(i < 20){
        Pthread_mutex_lock(&mtx);

        while(buffer[0] == -1){
            Pthread_cond_wait(&full, &mtx);
        }

        printf("%d  popped out of the buffer\n", buffer[0]);
        buffer[0] = -1;
        i++;

        Pthread_cond_signal(&empty);
        Pthread_mutex_unlock(&mtx);
        sleep(1);
    }
    return NULL;
}


int main (int argc, char **argv){
   int* buffer = malloc(sizeof(int));
    pthread_t prod;
    pthread_t cons;

    Pthread_create(&prod, NULL, producer, buffer);
    Pthread_create(&cons, NULL, consumer, buffer);

    //sleep(30);

    Pthread_join(prod, NULL);
    Pthread_join(cons, NULL);

    return 0;
}