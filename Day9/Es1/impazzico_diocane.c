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



//int update(fd_set *set);


int main(int argc, char **argv) {
    unlink(SOCKNAME);

    struct sockaddr_un server_address;
    strcpy(server_address.sun_path, SOCKNAME);
    server_address.sun_family = AF_UNIX;

    printf("starting server\n");

    int this_socket;
    int client_socket, current_socket, max_fd = 0;
    char payload[MAX_PAYLOAD_LENGTH];

    int error, read_bytes;
    fd_set read_set, general_set;

    printf("calling socket\n");
    this_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    printf("this socket %d", this_socket);

    //SYSCALL(error, select(max_fd+1, &read_set, NULL, NULL, NULL), "[SERVER-ERROR]: select: ");


    //}

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