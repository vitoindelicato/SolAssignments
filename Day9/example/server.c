#include <stdlib.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>


#define SOCKNAME "./mysocket.sck"
#define N 10
#define SYSCALL(r,c,e) \
    if((r=c)==-1) { perror(e); \
    exit(errno);       \
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


static void run_server(struct sockaddr_un *psa){
    int fd_sk, /* socket di connessione */ fd_c, /* socket di I/O con un client */ fd_num = 0, /* max fd attivo */fd; /* indice per verificare risultati select */
    char buf[N]; /* buffer messaggio */
    fd_set set; /* l’insiemedei file descriptor attivi */
    fd_set rdset; /* insieme fd attesi in lettura */
    int nread; /* numero carat */

    SYSCALL(fd_sk, socket(AF_UNIX,SOCK_STREAM,0), "error on socket: ");
    int r;
    SYSCALL(r, bind(fd_sk,(struct sockaddr *)psa,sizeof(*psa)), "error on bind: ");
    SYSCALL(r,listen(fd_sk,SOMAXCONN), "error on listen: ");

    if (fd_sk > fd_num) {
        fd_num = fd_sk;
    }
    FD_ZERO(&set);
    FD_SET(fd_sk,&set);

    while (1) {
        rdset = set;
        if (select(fd_num+1,&rdset,NULL,NULL,NULL) == -1) {
            perror("[SERERV-ERROR]: Select");
            exit(EXIT_FAILURE);
        }
        for (fd = 0; fd<=fd_num;fd++) {
            if (FD_ISSET(fd, &rdset)) {
                if (fd == fd_sk) {/* sock connect pronto */
                    SYSCALL(fd_c, accept(fd_sk, NULL, 0),"error on accept: ");
                    FD_SET(fd_c, &set);
                    if (fd_c > fd_num) fd_num = fd_c;
                }
                else {/* sock I/0 pronto */
                    SYSCALL(nread, read(fd, buf, N), "error on read: ");
                    if (nread == 0) {/* EOF client finito */
                        FD_CLR(fd, &set);
                        fd_num = update(&set);
                        close(fd);
                    }
                    else{
                        printf("[SERVER]:\t Got from client:  %s\n", buf);
                        SYSCALL(r,write(fd,"Bye!",5), "[SERVER-ERROR]: write: ");
                    }
                }
            }
        }
    }
}



static void run_client(struct sockaddr_un * psa) {
    if (fork()==0) { /* figlio, client */
        int fd_skt;
        char buf[N];
        fd_skt = socket(AF_UNIX, SOCK_STREAM, 0);
        printf("[CLIENT]:\tTrying to connect to server\n");

        while (connect(fd_skt, (struct sockaddr *) psa, sizeof(*psa)) == -1) {
            if (errno == ENOENT) {
                printf("[CLIENT]:\tSocket not found, retrying...\n");
                sleep(1);
            }
            else exit(EXIT_FAILURE);
        }

        printf("[CLIENT]:\tSuccessfully connected to server\n");
        int n = write(fd_skt,"Hallo!",7);
        if (n < 0){
            perror("[CLIENT-ERROR]:Write");
        }
        n = 0;
        n = read(fd_skt, &buf, N);
        if (n < 0){
            perror("[CLIENT-ERROR]: Read");
        }
        printf("[CLIENT]\t Got from server: %s\n", buf) ;
        close(fd_skt);
        exit(EXIT_SUCCESS);
    }
}




int main(int argc, char *argv[]) {
    struct sockaddr_un sa;
    strcpy(sa.sun_path, SOCKNAME);
    sa.sun_family=AF_UNIX;

    //printf("%d", _SSIZE_MAX);

    for(int i=0; i< 4; i++) {
        run_client(&sa);
    }

    run_server (&sa);

    return 0;
}


