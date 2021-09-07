#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char const *argv[])
{
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8080);

    int sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock == -1)
    {
        perror("socket");
    }
    if(connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
    {
        perror("connect");
    }
    char buff[255];
    int len = 0;
    len = read(sock,buff,255);
    buff[len] = '\0';
    printf("MSG(%d): %s \n", len, buff);
    return 0;
}
