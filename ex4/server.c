#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
// server listining on port 8080

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
    setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &(int){1}, sizeof(int));
    if(bind(sock,(struct sockaddr *) &servaddr, sizeof(servaddr)) == -1)
    {
        perror("bind");
    }
    if(listen(sock,3) < 0)
    {
        perror("listen");
    }
    int conn = 0;
    conn = accept(sock,(struct sockaddr *) NULL, NULL);
    char* buff = "Hello World!";
    send(conn, buff, strlen(buff),0);
    close(sock);
    return 0;
}
