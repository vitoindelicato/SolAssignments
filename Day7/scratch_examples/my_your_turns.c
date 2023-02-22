#include <unistd.h>
#include <stdio.h>
#include "Pthread.h"
#include <stdlib.h>

void* my_turn(void* arg){
    int *something = malloc(sizeof(int));
    *something = 10;
    for (int i = 0; i < 10; i++){
        printf("My turn %d\n", i);
        *something += 1;
        sleep(1);
    }
    return (void*) something;
}

void yourturn(){
    for (int i = 0; i < 5; i++){
        printf("Your turn %d\n", i);
        sleep(1);
    }
    //return NULL;
}


int main(void){
    int *thread_result;

    pthread_t myturn;
    Pthread_create(&myturn, NULL, my_turn, NULL);

    yourturn();

    pthread_join(myturn, (void*)&thread_result);
    printf("something value is: %d\n", *thread_result);

    return 0;
}