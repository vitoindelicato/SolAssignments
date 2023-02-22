#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>



int main (int argc, char **argv){

    if(argc != 3){
        perror("Usage: ./sleepy -s <seconds>");
        exit(EXIT_FAILURE);
    }

    //int secs = strtol(argv[1], NULL, 10);

    //char *args[1] = secs;

    pid_t pid = fork();

    if(pid == 0){
        execv("/bin/sleep", &argv[1]);
        perror("Cannot exec sleep");
    }
    else if (pid > 0){
        int status;
        if(waitpid(pid, &status, 0) == -1){
            perror("son failed");
            exit(EXIT_FAILURE);
        }
        else {
            printf("Here's the father with id: %d\nMy son is %d\n", getpid(), pid);
        }
    }
    else{
        perror("fork failed");
        exit(1);
    }



    return 0;
}