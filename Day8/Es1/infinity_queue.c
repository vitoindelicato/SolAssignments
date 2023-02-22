#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N_PROD 4
#define N_CONS 4
#define INFINITE 500

int rear = 0;
int reader_cursor = 0;
int buffer[INFINITE];
int ready = 0;
uint seed;

void *writer(void *arg);
void *reader(void *arg);


pthread_mutex_t buff = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ready_cond = PTHREAD_COND_INITIALIZER;



int main(int argc, char **argv){
    pthread_t writers[N_PROD];
    pthread_t readers[N_CONS];

    seed = time(NULL);

    int k = atoi(argv[1]);

    if(k < N_PROD){
        perror("number of messages must be >= number of consumers\n");
    }

    for(int i = 0; i < N_PROD; i++){
        pthread_create(&writers[i], NULL, writer, &k);
    }

    for(int i = 0; i < N_CONS; i++){
        pthread_create(&readers[i], NULL, reader, &k);
    }

    for(int i = 0; i < N_PROD; i++){
        pthread_join(writers[i], NULL);
    }


    pthread_mutex_lock(&buff);
    //rear +=1;
    buffer[rear] = -1;
    ready = 1;
    pthread_cond_signal(&ready_cond);
    pthread_mutex_unlock(&buff);

    for(int i = 0; i < N_CONS; i++){
        pthread_join(readers[i], NULL);
    }

    return 0;
}


void* writer(void *arg){
    int *k = (int*)arg;
    int to_write = *k / N_PROD;

    for (int i = 0; i < to_write; i++){
        pthread_mutex_lock(&buff);
        buffer[rear] = rand_r(&seed) % 100;
        rear+=1;
        pthread_mutex_unlock(&buff);
    }
    return NULL;
}


void* reader(void *arg){
    //int *k = (int*)arg;
    while(1){
        pthread_mutex_lock(&buff);
        while(!ready){
            pthread_cond_wait(&ready_cond, &buff);
        }

        if(buffer[reader_cursor] == -1){
            printf("Reader exiting\n");
            pthread_cond_signal(&ready_cond);
            pthread_mutex_unlock(&buff);
            pthread_exit(NULL);
        }
        else{
            printf("%d\n", buffer[reader_cursor]);
            reader_cursor += 1;
            pthread_mutex_unlock(&buff);
        }

    }

    return NULL;
}
