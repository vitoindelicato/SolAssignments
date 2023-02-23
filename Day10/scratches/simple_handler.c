#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>


static void gestore (int signum) {
    printf("Ricevuto segnale %d\n", signum);
    exit(EXIT_FAILURE);
}

int main (void){

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));

    //sa.sa_handler = gestore;  /* to handle signal with function */
    sa.sa_handler = SIG_IGN;  /* to ignore signal */

    sigaction(SIGINT, &sa, NULL);

    for (int i = 0; i < 100; i++) {
        printf("%d\n", i);
        sleep(1);
    }

}