#include <sys/un.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define SOCKNAME "./diomerdoso.sck"
#define MAX_PAYLOAD_LENGTH 100

#define SYSCALL(r,c,e) \
    if((r=c)==-1) { perror(e); \
    exit(errno);       \
    }



int update(fd_set *set);


int main(int argc, char **argv){
    unlink(SOCKNAME);

    struct sockaddr_un server_address;
    strcpy(server_address.sun_path, SOCKNAME);
    server_address.sun_family=AF_UNIX;

    printf("starting server\n");

    int this_socket;
    int client_socket, current_socket, max_fd = 0;
    char payload[MAX_PAYLOAD_LENGTH];

    int error, read_bytes;
    fd_set read_set, general_set;

    printf("calling socket\n");
    SYSCALL(this_socket, socket(AF_UNIX, SOCK_STREAM, 0), "[SERVER-ERROR]: socket creation: ");
    printf("this socket %d", this_socket);


    SYSCALL(error, bind(this_socket, (struct sockaddr*) &server_address, sizeof(server_address)), "[SERVER-ERROR]: bind: ");
    SYSCALL(error, listen(this_socket, 5), "[SERVER-ERROR]: listen: ");

    printf("this socket %d", this_socket);
    if (this_socket > max_fd){
        max_fd = this_socket;
    }

    FD_ZERO(&general_set);
    FD_SET(this_socket, &general_set);

    while(1){
        read_set = general_set;

        SYSCALL(error, select(max_fd+5, &read_set, NULL, NULL, NULL), "[SERVER-ERROR]: select: ");

        for(current_socket = 0; current_socket < max_fd; current_socket++){

            if(FD_ISSET(current_socket, &read_set)){

                if(current_socket == this_socket){
                    printf("accepting new client\n");
                    SYSCALL(client_socket, accept(this_socket, NULL, 0), "[SERVER-ERROR]: accept: ");
                    FD_SET(client_socket, &general_set);
                    //FD_SET(client_socket, &read_set);

                    if(client_socket > max_fd){
                        max_fd = client_socket;
                    }

                }
                else{
                    printf("accepted new client\n");
                    printf("reading from client\n");
                    SYSCALL(read_bytes, read(current_socket, payload, MAX_PAYLOAD_LENGTH), "[SERVER-ERROR]: read: ");

                    if(read_bytes == 0){
                        FD_CLR(current_socket, &general_set);
                        max_fd = update(&general_set);
                        close(current_socket);
                    }
                    else{
                        printf("[SERVER-RECEIVED]: %s\n", payload);
                        SYSCALL(error, write(current_socket, "message from server!!!!", 24), "[SERVER-ERROR]: write: ");
                    }
                }
            }
        }
    }


    return 0;
}




int update(fd_set *set) {
    //printf("updateing fd\n");
    int fd_num = 0, fd;
    for (fd = 0; fd <= FD_SETSIZE; fd++) {
        if (FD_ISSET(fd, set)) {
            if (fd > fd_num) fd_num = fd;
        }
    }
    return fd_num;
}