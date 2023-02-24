#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#define SYSCALL(r,c,e) { \
    r=c;                 \
    if(r==-1){perror(e); _exit(errno);} \
    }



volatile sig_atomic_t sigint_count = 0;
volatile sig_atomic_t sigtstp_count = 0;

int err;


static void sigint_handler(int signum){
    sigint_count+=1;
}

static void sigtstp_handler(int signum){
    SYSCALL(err, write(STDOUT_FILENO, (void*)sigint_count, sizeof(volatile sig_atomic_t)), "error on write");
    sigtstp_count++;
}

void sigalrm_handler(int signum){
    write(STDOUT_FILENO, "Time's up! Process terminated by SIGALRM\n", 42);
    _exit(EXIT_SUCCESS);
}


int main(int argc, char **argv){

    struct sigaction sigint, sigtstp, sigalrm;

    memset(&sigint, 0, sizeof(sigint));
    memset(&sigtstp, 0, sizeof(sigtstp));
    memset(&sigalrm, 0, sizeof(sigalrm));


    sigint.sa_handler = sigint_handler;
    if ( sigaction(SIGINT, &sigint, NULL) == -1 ) {
        perror("Couldn't set SIGINT handler");
        exit(EXIT_FAILURE);
    }

    sigtstp.sa_handler = sigtstp_handler;
    if ( sigaction(SIGTSTP, &sigtstp, NULL) == -1 ) {
        perror("Couldn't set SIGTSTP handler");
        exit(EXIT_FAILURE);
    }


    sigalrm.sa_handler = sigalrm_handler;
    if ( sigaction(SIGALRM, &sigalrm, NULL) == -1 ) {
        perror("Couldn't set SIGALRM handler");
        exit(EXIT_FAILURE);
    }


    while(sigtstp_count < 3){
    }

    char response = 'n';
    printf("Do you want to terminate the program? (y/n) ");
    scanf("%c", &response);

    if(response == 'y'){
        exit(EXIT_SUCCESS);
    }
    else{
        alarm(5);
        while(1){

        }
    }


    return 0;
}