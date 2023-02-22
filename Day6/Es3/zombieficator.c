#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>



int main (int argc, char **argv) {

    if (argc != 2) {
        perror("Usage: ./zombificator N");
        exit(EXIT_FAILURE);
    }
    int N = strtol(argv[1], NULL, 10);

    for(int i = 0; i < N; i++){
        if (fork() == 0){
            exit(0);
        }
    }

    execl("/bin/sleep", "-s", "1000", NULL);



    return 0;
}
