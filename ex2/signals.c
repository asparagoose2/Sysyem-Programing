#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

int counter = 0;
int counterLIMIT = 0;
int flag = 0;
char* p;

void my_sig_int(int sig_number) {
    flag = 1;
    if (!(++counter < counterLIMIT))
    {
        printf("\nI am dead (X_X)\n");
        exit(1);
    }
    
}

int main(int argc, char const *argv[])
{
    if(argc != 2) {
        printf("USAGE: <file-name> <counter-value>\n");
        return -1;
    }
    if(*argv[1] < 1) {
        printf("invalid counter value!\n");
        return -1;
    }

    if(signal(SIGINT, my_sig_int) < 0) {
        perror("signal");
        return -1;
    }
    counterLIMIT = strtol(argv[1],&p,10);

    while(1) {
        if(flag) {
            printf("\nYou won't kill me! attempt #%d\n", counter);
            flag = 0;
        }
    }
    return 0;
}
