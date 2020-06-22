#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>



pid_t pid;

static void sigint(int signo) {
    signal(signo, SIG_IGN); /*ignore other SIGINT signals */
    printf("received SIGINT in parent process\n");
    fflush(stdout);
    kill(pid, SIGINT);
    printf("Child process killed\n");
    fflush(stdout);
    signal(signo, sigint); /*reinstall the signal handler */
}

static void sigtstp(int signo) {
    signal(signo, SIG_IGN);
    printf("Received SIGTSTP in parent process\n");
    fflush(stdout);
    kill(pid, SIGTSTP);
    printf("Child process suspended\n");
    fflush(stdout);
    signal(signo, sigtstp);
    
}

int main(int argc, char **argv) {
    
    int status;
    
    if (argc < 2) {
        printf("Usage: %s <command> [args]\n", argv[0]);
        exit(-1);
    }
    
    /* set up handler to intercept SIGINT */
    if (signal(SIGINT, sigint) == SIG_ERR) {
        printf("Unable to catch SIGINT\n");
    }
    
    if (signal(SIGTSTP, sigtstp) == SIG_ERR) {
        printf("Unable to catch SIGTSTP\n");
    }
    pid = fork();
    if (pid == 0){ /* this is the child process */
        execvp(argv[1], &argv[1]);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else if (pid > 0) { /* this is the parent process */
        printf("[%ld]: Wait for the child process to terminate\n", (long)getpid());
//        wait(&status);
        for ( ; ; )
            pause;
        printf("[%ld]: Exiting the program.....\n", (long)getpid());
//        if (WIFEXITED(status)) { /* child process terminated normally */
//            printf("Child process exited with status = %d\n", WEXITSTATUS(status));
        } else {
            printf("fork");
            exit(EXIT_FAILURE);
        }
    }

