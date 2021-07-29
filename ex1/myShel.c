#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

void cat(int argc, char* argv[]) 
{
    FILE* output = stdout;
    if(argc < 2 || argc > 4)
    {
        fprintf(stderr, "%s\n", "USAGE: cd <path> [ > output file ]" );
        return;
    }

    if (argc == 4 && (strcmp(argv[2],">")==0))
    {
        output = fopen(argv[3],"w");
        int err = errno;
        if (err != 0) {
            perror("fopen");
            errno = 0;
            return;
        }
    }

    int err = errno;
    if (err != 0) {
        perror("fopen");
        errno = 0;
        return;
    }
    
    int fd = open(argv[1],O_RDONLY);

    err = errno;
    if (err != 0) {
        perror("open");
        errno = 0;
        return;
    }

    char buff[256];

    while (read(fd, &buff, 255))
    {
        fprintf(output,"%s",buff);
    }
    fflush(output);
    fprintf(stdout,"\r");

    if (output != stdout)
    {
        fclose(output);
    }

    close(fd);
    
    return;    

}

void cd(int argc, char* argv[]) 
{
    if(argc != 2)
    {
        fprintf(stderr, "%s\n", "USAGE: cd <path>" );
        return;
    }
    chdir(argv[1]);
    int err = errno;
    if (err != 0) {
        perror("Change Directory (chdir)");
        errno = 0;
        return;
    }
}

void ls(int argc, char* argv[]) 
{
    if(argc < 2)
    {
        fprintf(stderr, "%s\n", "USAGE: ls <file-path>" );
        return;
    }

    FILE* output = stdout;

    if (argc > 3 && (strcmp(argv[2],">")==0))
    {
        output = fopen(argv[3],"w");
        int err = errno;

        if (err != 0) {
            perror("Open File (fopen)");
            errno = 0;
            return;
        }
    }
    

    DIR *dir = opendir(argv[1]);
    struct dirent *content = NULL;

    if(NULL == dir)
    {
        fprintf(stderr, "%s\n", "Can't open directory!");
        errno = 0;
        return;
    }

    while ((content = readdir(dir)) != NULL) {
        fprintf(output, "%s\n", content->d_name);
    }

    int err = errno;

    if (err != 0) {
        perror("Read Dir / Print (readdir / fprintf)");
        errno   = 0;
        return;
    }

    closedir(dir);

    if (output != stdout)
    {
        fclose(output);
    }
    

    return;
}

int argvMaker(char* inputLine, char ***argv)
{

    if(NULL == inputLine || strlen(inputLine) < 1)
    {
        *argv = NULL;
        return 0;
    }
    char* token = NULL;
    int argc = 0;
    *argv = malloc(sizeof(char*));
    
    token = strtok(inputLine, " ");

    while(token != NULL)
    {
        char* var = malloc(sizeof(token)+1);
        strcpy(var, token);
        char* c = NULL;
        if((c = strchr(var,'\n')))
        {
            *c = '\0';
        }

        *argv = realloc(*argv,sizeof(char*)*++argc);
        (*argv)[argc-1] = var;
        token = strtok(NULL, " ");
    }

    return argc;
}

int main(int argc, char* argv[])
{
    char path[100];
    size_t size = 256;
    char* buff = (char*) malloc(size);
    getcwd(path,100);
    printf("%s>",path);
    getline(&buff,&size,stdin);
    while(strcmp(buff,"exit\n") != 0)
    {
        char** arguments;
        int argumentsCount = argvMaker(buff,&arguments);

        pid_t pid;

        if(strcmp(arguments[0],"ls") == 0)
        {
            if((pid = fork())<0)
            {
                perror("fork");
                errno = 0;
            } else if(!pid) {
                printf("Child PID is: %d\n", getpid());
                ls(argumentsCount,arguments);
                exit(0);

            } else {
                int status;
                if ((pid = waitpid(pid, &status,0)) < 0)
                {
                    perror("waitpid");
                    errno = 0;
                }
            }
        } else if(strcmp(arguments[0],"cd") == 0) {
            cd(argumentsCount,arguments);
        } else if(strcmp(arguments[0],"cat") == 0) {


            if((pid = fork())<0)
            {
                perror("fork");
                errno = 0;
            } else if(!pid) {
                printf("Child PID is: %d\n", getpid());
                cat(argumentsCount,arguments);
                exit(0);

            } else {
                int status;
                if ((pid = waitpid(pid, &status,0)) < 0)
                {
                    perror("waitpid");
                    errno = 0;
                }
            }

        } else {

            printf("Command '%s' not found!\n", arguments[0]);
        }
        getcwd(path,100);
        printf("%s>",path);
        getline(&buff,&size,stdin);    
    }
    return 0;
}