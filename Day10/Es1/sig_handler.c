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
    //write(STDOUT_FILENO, (int*)&sigint_count, sizeof(sigint_count));
    sigtstp_count++;
}

void sigalrm_handler(int signum){
    write(STDOUT_FILENO, "Time's up! Process terminated by SIGALRM\n", 42);
    _exit(EXIT_SUCCESS);
}


int main(int argc, char **argv){

    sigset_t set;
    int sig;
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));

    sigemptyset(&set);
    sigaddset(&set, SIGTSTP);
    // sigprocmask(SIG_BLOCK, &set, NULL); --> still have to fully understand this command
    // pthread_sigmask(SIG_SETMASK, &set, NULL); -->right now this is not necessary.


    sa.sa_handler = sigint_handler;
    if ( sigaction(SIGINT, &sa, NULL) == -1 ) {
        perror("Couldn't set SIGINT handler");
        exit(EXIT_FAILURE);
    }

    /* --> this is moved on main, I wait for ctrl-z, and I execute handler in main with no limitations.
     * with this handler, i wasn't able to properly print the number of SIGINT received.

    sa.sa_handler = sigtstp_handler;
    if ( sigaction(SIGTSTP, &sa, NULL) == -1 ) {
        perror("Couldn't set SIGTSTP handler");
        exit(EXIT_FAILURE);
    }
     */


    sa.sa_handler = sigalrm_handler;
    if ( sigaction(SIGALRM, &sa, NULL) == -1 ) {
        perror("Couldn't set SIGALRM handler");
        exit(EXIT_FAILURE);
    }


    while(1){

        sigwait(&set, &sig);
        printf("Arrived SIGTSTP, number of SIGINT is:   %d\n", sigint_count);

        if(sigtstp_count < 2){
            sigtstp_count += 1;
            continue;
        }
        else{
            break;
        }
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