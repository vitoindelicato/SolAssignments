#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>


void formatter (int i, int n){
    for(int j = 0; j < (n-i); j++){printf("- ");}
}

void forker(int i, int n){
    if(n - i > 0){
        formatter(i, n);
        printf("[%d] here!: Creating child\n", getpid());

        pid_t pid = fork();
        if(pid == 0) {
            forker(i+1, n);
        }
        else if(pid > 0){
            int status;
            if (waitpid(pid, &status, 0) == -1) {perror("waitpid:"); exit(EXIT_FAILURE);}
            formatter(i, n);
            printf("[%d]: finished successfully\n",getpid());
        }
    }
    else if(n - i == 0){
        printf("[%d]: I'm the last one!\n", getpid());
        printf("[%d]: finished successfully\n",getpid());
    }
}


int main(int argc, char **argv){
    if (argc !=2){
        perror("missing number of processes");
    }
    int n = strtol(argv[1], NULL, 10);
    if (n <= 1) {
        perror("n must be >1");
    }

    forker(0, n);





    return 0;
}


