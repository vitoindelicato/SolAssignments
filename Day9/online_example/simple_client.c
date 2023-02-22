#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>

#define DATA_BUFFER "Mona Lisa was painted by Leonardo da Vinci"
#define MAX_PAYLOAD_LENGTH 100
#define SOCKNAME "./diomerdoso.sck"

int main () {
    struct sockaddr_un saddr;
    saddr.sun_family=AF_UNIX;
    strcpy(saddr.sun_path, SOCKNAME);
    int fd, ret_val;


    /* Step1: create a TCP socket */
    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1) {
        fprintf(stderr, "socket failed [%s]\n", strerror(errno));
        return -1;
    }
    printf("Created a socket with fd: %d\n", fd);


    /* Step2: connect to the TCP server socket */
    //ret_val = connect(fd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_un));

    while((ret_val = connect(fd, (struct sockaddr*) &saddr, sizeof(struct sockaddr_un)) == -1)){
        if (errno == ENOENT){
            printf("[CLIENT]:\tSocket not found, retrying...\n");
            sleep(1);
        }
        else exit(EXIT_FAILURE);
    }
    /*
    if (ret_val == -1) {
        fprintf(stderr, "connect failed [%s]\n", strerror(errno));
        close(fd);
        return -1;
    }*/

    printf("The Socket is now connected\n");

    //printf("Let us sleep before we start sending data\n");
    //sleep(5);

    /* Next step: send some data */
    printf("Insert operation:   ");
    char payload[MAX_PAYLOAD_LENGTH];
    fgets(payload, MAX_PAYLOAD_LENGTH, stdin);
    payload[strlen(payload)] = '\0';
    ret_val = write(fd,payload, strlen(payload));

    //printf("Successfully sent data (len %d bytes): %s\n",
    //       ret_val, payload);

    char response[MAX_PAYLOAD_LENGTH];

    ret_val = read(fd, response, MAX_PAYLOAD_LENGTH);
    printf("Result: %s\n", response);
    /* Last step: close the socket */
    close(fd);
    return 0;
}