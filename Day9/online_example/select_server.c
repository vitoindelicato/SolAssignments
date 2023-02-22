#include <stdio.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define DATA_BUFFER 5000
#define MAX_CONNECTIONS 10
#define MAX_PAYLOAD_LENGTH 100
#define SOCKNAME "./diomerdoso.sck"

#define WRITE_END 1
#define READ_END 0

#define SYSCALL(r, c, e) \
    if((r = c) == -1) { perror(e); exit(errno); }

int create_server_socket(struct sockaddr_un *saddr) {
    int fd, ret_val;

    /* Step1: create a TCP socket */
    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1) {
        fprintf(stderr, "socket failed [%s]\n", strerror(errno));
        return -1;
    }
    printf("Created a socket with fd: %d\n", fd);

    /* Initialize the socket address structure */



    /* Step2: bind the socket to port 7000 on the local host */
    ret_val = bind(fd, (struct sockaddr *)saddr, sizeof(struct sockaddr_un));
    if (ret_val != 0) {
        fprintf(stderr, "bind failed [%s]\n", strerror(errno));
        close(fd);
        return -1;
    }

    /* Step3: listen for incoming connections */
    ret_val = listen(fd, 5);
    if (ret_val != 0) {
        fprintf(stderr, "listen failed [%s]\n", strerror(errno));
        close(fd);
        return -1;
    }
    return fd;
}



char* bc(char* payload){
    int input_pipe[2], output_pipe[2];
    char *result = malloc(sizeof(char)*MAX_PAYLOAD_LENGTH);


    if(pipe(input_pipe)< 0 || pipe(output_pipe) < 0) {
        perror("pipe:  ");
    }

    ssize_t k = write(input_pipe[WRITE_END], payload, strlen(payload));

    if(k < 0){
        perror("write");
    }

    pid_t pid = fork();

    if(pid == -1) {
        perror("fork");
    }
    else if(pid > 0) {

        close(output_pipe[WRITE_END]);
        close(input_pipe[READ_END]);


        ssize_t r = read(output_pipe[READ_END], result, MAX_PAYLOAD_LENGTH);

        if(r < 0){
            perror("read");
        }

        //printf("result:  %s\n", result);

        close(output_pipe[READ_END]);
        close(input_pipe[WRITE_END]); //closing bc stdin in order to terminate child process

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
    }
    return result;

}



int main (int argc, char** argv) {
    fd_set read_fd_set, write_fd_set;
    unlink(SOCKNAME);
    struct sockaddr_un new_addr;
    new_addr.sun_family=AF_UNIX;
    strcpy(new_addr.sun_path, SOCKNAME);

    int server_fd, new_fd, ret_val, i;
    socklen_t addrlen;
    char buf[DATA_BUFFER];
    int all_connections[MAX_CONNECTIONS];

    /* Get the socket server fd */
    server_fd = create_server_socket(&new_addr); //socket --> bind --> listen
    if (server_fd == -1) {
        fprintf(stderr, "Failed to create a server\n");
        return -1;
    }

    /* Initialize all_connections and set the first entry to server fd */
    for (i=0;i < MAX_CONNECTIONS;i++) {
        all_connections[i] = -1;
    }
    all_connections[0] = server_fd;

    while(1) {
        FD_ZERO(&read_fd_set);
        FD_ZERO(&write_fd_set);
        /* Set the fd_set before passing it to the select call */
        for (i = 0; i < MAX_CONNECTIONS; i++) {
            if (all_connections[i] >= 0) {
                FD_SET(all_connections[i], &read_fd_set);
                //FD_SET(all_connections[i], &write_fd_set);
            }
        }

        /* Invoke select() and then wait! */
        printf("\nUsing select() to listen for incoming events\n");
        ret_val = select(MAX_CONNECTIONS+1, &read_fd_set, &write_fd_set, NULL, NULL);

        /* select() woke up. Identify the fd that has events */
        if (ret_val >= 0 ) {
            printf("Select returned with %d\n", ret_val);
            /* Check if the fd with event is the server fd */
            if (FD_ISSET(server_fd, &read_fd_set)) {
                /* accept the new connection */
                printf("Returned fd is %d (server's fd)\n", server_fd);
                new_fd = accept(server_fd, (struct sockaddr*)&new_addr, &addrlen);
                if (new_fd >= 0) {
                    printf("Accepted a new connection with fd: %d\n", new_fd);
                    for (i = 0; i < MAX_CONNECTIONS; i++) {
                        if (all_connections[i] < 0) {
                            all_connections[i] = new_fd;
                            break;
                        }
                    }
                } else {
                    fprintf(stderr, "accept failed [%s]\n", strerror(errno));
                }
                ret_val--;
                if (!ret_val) continue;
            }

            /* Check if the fd with event is a non-server fd */
            for (i = 1; i < MAX_CONNECTIONS; i++) {

                if (all_connections[i] > 0) {
                    char *operation = malloc(sizeof(char)*MAX_PAYLOAD_LENGTH);

                    if (FD_ISSET(all_connections[i], &read_fd_set)) {
                        /* read incoming data */
                        printf("Returned fd is %d [index, i: %d]\n", all_connections[i], i);

                        ret_val = read(all_connections[i], operation, MAX_PAYLOAD_LENGTH);

                        if (ret_val == 0) {
                            printf("Closing connection for fd:%d\n", all_connections[i]);
                            FD_SET(all_connections[i], &write_fd_set);
                            //close(all_connections[i]);
                            //all_connections[i] = -1; /* Connection is now closed */
                        }
                        if (ret_val > 0) {
                            printf("Received data (len %d bytes, fd: %d): %s\n",
                                   ret_val, all_connections[i], buf);
                            FD_SET(all_connections[i], &write_fd_set);
                            //close(all_connections[i]);

                        }
                        if (ret_val == -1) {
                            printf("recv() failed for fd: %d [%s]\n",
                                   all_connections[i], strerror(errno));
                            close(all_connections[i]);
                            break;
                        }
                    }
                    if(FD_ISSET(all_connections[i], &write_fd_set)){
                        /* we can write on client */
                        printf("Write something to send client[%d]:   ", all_connections[i]);
                        char *response;
                        response = bc(operation);
                        free(operation);

                        int nbytes = write(all_connections[i], response, strlen(response));

                        if(nbytes == -1){
                            free(response);
                            printf("write() failed for fd: %d [%s]\n",
                                   all_connections[i], strerror(errno));
                            break;
                        }
                        else{
                            free(response);
                            printf("write() success for fd: %d \n",
                                   all_connections[i]);
                            FD_CLR(all_connections[i], &write_fd_set);
                            close(all_connections[i]);
                            all_connections[i] = -1;
                        }
                    }
                    //ret_val--;
                    //if (!ret_val) continue;
                }
            } /* for-loop */
        } /* (ret_val >= 0) */
    } /* while(1) */

    /* Last step: Close all the sockets
    for (i=0;i < MAX_CONNECTIONS;i++) {
        if (all_connections[i] > 0) {
            close(all_connections[i]);
        }
    }
    return 0;
     */
}