#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>


int main(int argc, char const *argv[])
{
    pid_t* arr;
    arr = mmap(NULL,3*sizeof(pid_t), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    printf("Father PID: %d \n", getpid());
    int pid;
    if (pid = fork())
    {
        if(pid = fork())
        {
            arr[0] = getpid();
            int status;
            if(waitpid(-1,&status,0) < 0)
            {
                perror("waitpid");
                exit(1);
            }
            printf("%d, %d, %d\n", arr[0], arr[1], arr[2]);
        } else {
            if(pid < 0)
            {
                perror("fork");
                exit(1);
            }
            arr[2] = getpid();
        }
    } else {
        if(pid < 0)
        {
            perror("fork");
            exit(1);
        }
        arr[1] = getpid();
    }
    return 0;

}
