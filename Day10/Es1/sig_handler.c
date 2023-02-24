#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

volatile sig_atomic_t sigint_count = 0;
volatile sig_atomic_t sigtstp_count = 0;


static void sigint_handler(int signum){
    sigint_count++;
}

static void sigtstp_handler(int signum){
    write(STDOUT_FILENO, (void *) &sigint_count, sizeof(sigint_count));
    sigtstp_count++;
    exit(EXIT_SUCCESS);
}

void sigalrm_handler(int signum){
    write(STDOUT_FILENO, "Time's up! Process terminated by SIGALRM", 41);
    exit(EXIT_SUCCESS);
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
    }




    return 0;
}