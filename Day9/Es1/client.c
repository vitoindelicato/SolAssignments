#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKNAME "./diomerdoso.sck"
#define MAX_PAYLOAD_LENGTH 100

#define SYSCALL(r,c,e) \
    if((r=c)==-1) { perror(e); \
    exit(errno);       \
    }

int main(int argc, char **argv){

    struct sockaddr_un server_address;
    strncpy(server_address.sun_path, SOCKNAME, strlen(SOCKNAME)+1);
    server_address.sun_family=AF_UNIX;

    int this_socket;
    char payload[MAX_PAYLOAD_LENGTH];
    char response[MAX_PAYLOAD_LENGTH];
    int write_bytes;
    int read_bytes;

    SYSCALL(this_socket, socket(AF_UNIX, SOCK_STREAM, 0), "[CLIENT-ERROR]: socket creation: ");

    while(connect(this_socket, (struct sockaddr*) &server_address, sizeof(server_address)) == -1){
        if (errno == ENOENT){
            printf("[CLIENT]:\tSocket not found, retrying...\n");
            sleep(1);
        }
        else exit(EXIT_FAILURE);
    }

    printf("insert message:   ");
    fgets((char*) &payload, MAX_PAYLOAD_LENGTH, stdin);
    payload[strlen(payload)] = '\0';
    while(strcmp(payload, "quit") != 0){
        SYSCALL(write_bytes, write(this_socket, payload, strlen(payload)), "[CLIENT-ERROR]: write: ");
        SYSCALL(read_bytes, read(this_socket, response, MAX_PAYLOAD_LENGTH), "[CLIENT-ERROR]: read: ");
        printf("[CLIENT]:\tReceived: %s\n", response);
        sleep(1);
        printf("insert message:   ");
    }

    close(this_socket);

    return 0;
}