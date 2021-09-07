#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    int num = 0;
    char* p;
    if(argc != 2) 
    {
        printf("USAGE: <file-name> <parametrt>\n");
        exit(1);
    }
    num = strtol(argv[1],&p,10);
    if(num < 0 || num > 9) 
    {
        printf("Paremeter must be between 0 and 9\n");
        exit(1);
    }
    int fd;
    int file_offset = 0;
    if((fd = open("MY-EXERCISE.out",O_CREAT | O_WRONLY | O_TRUNC)) < 0) 
    {
        perror("open");
        exit(1);
    }
    char toPrint[2] = "1";
    for (size_t i = num; i > 0; i--)
    {
        if(!(i == num)) 
        {
            if ((file_offset = lseek(fd,i/2, SEEK_SET)) == -1)
            {
                perror("lseek");
                exit(1);
            }

        }
        for (size_t j = 0; j <= i; j++)
        {
            toPrint[0] = i + '0';
            if (write(fd,toPrint ,strlen(toPrint)) != strlen(toPrint))
            {
                perror("write");
                exit(1);
            }  
        }
            
        
    }
    
    
    
    return 0;
}
