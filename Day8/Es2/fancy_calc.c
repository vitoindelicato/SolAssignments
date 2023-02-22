#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<sys/wait.h>
#include <fcntl.h>

#define WRITE_END 1
#define READ_END 0

#define BUFFER_SIZE 1024

int main() {
    int input_pipe[2], output_pipe[2];
    char cmd[BUFFER_SIZE];

    printf("insert operation:  ");
    fgets(cmd, BUFFER_SIZE, stdin);
    cmd[strlen(cmd)] = '\0';
    printf("%s\n", cmd);

    while(strcmp(cmd, "exit")!=0) {
        if(pipe(input_pipe)< 0 || pipe(output_pipe) < 0) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        ssize_t k = write(input_pipe[WRITE_END], cmd, strlen(cmd));

        if(k < 0){
            perror("write");
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();

        if(pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if(pid > 0) {

            close(output_pipe[WRITE_END]);
            close(input_pipe[READ_END]);

            char* result = malloc(sizeof(char)*100);
            ssize_t r = read(output_pipe[READ_END], result, 100);
            if(r < 0){
                perror("read");
                exit(EXIT_FAILURE);
            }

            printf("result:  %s\n", result);
            free(result);

            close(output_pipe[READ_END]);
            close(input_pipe[WRITE_END]); //closing bc stdin in order to terminate child process

            printf("insert operation:  ");
            fgets(cmd, BUFFER_SIZE, stdin);
            cmd[strlen(cmd)] = '\0';
        }
        else if(pid == 0) {
            close(output_pipe[READ_END]);
            close(input_pipe[WRITE_END]);

            dup2(input_pipe[READ_END], STDIN_FILENO);
            dup2(output_pipe[WRITE_END], STDOUT_FILENO);
            dup2(output_pipe[WRITE_END], STDERR_FILENO);

            char *args[] = {"bc", "-lq", NULL};
            execvp(args[0], args);

            // If execvp returns, an error occurred
            perror("execvp");
            exit(1);
        }
    }

    return 0;
}