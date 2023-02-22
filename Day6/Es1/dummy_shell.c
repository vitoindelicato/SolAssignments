#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LEN 100

int main(void) {
    char input[MAX_LEN];
    char *args[MAX_LEN];
    int i;

    while (1) {
        printf("dummyshell> ");
        fgets(input, MAX_LEN, stdin);
        input[strlen(input) - 1] = '\0';

        if (strcmp(input, "exit") == 0) {
            break;
        }

        i = 0;
        args[i] = strtok(input, " ");

        while (args[i] != NULL) {
            i++;
            args[i] = strtok(NULL, " ");
        }

        pid_t pid = fork();
        if (pid == 0) {
            execvp(args[0], args);
            perror("execvp failed");
            exit(1);
        }

        else if (pid > 0) {
            wait(NULL);
        }
        else {
            perror("fork failed");
            exit(1);
        }
    }
    return 0;
}
