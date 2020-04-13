#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <errno.h>

#define MAXBUF  1024

int main(int argc, char *argv[])
{
    int sockfd, bytes_read;
    struct sockaddr_in dest;
    char buffer[MAXBUF];

    if(argc != 4) {
        perror("usage: testport <IP> <PORT> <send-msg>\n");
        return -1;
    }
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket");
        return -1;
    }

    bzero(&dest, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr=inet_addr(argv[1]);
    dest.sin_port=htons(atoi(argv[2]));

    if(connect(sockfd, (struct sockaddr*)&dest, sizeof(dest)) != 0) {
        perror("Connect");
        return -1;
    }

    sprintf(buffer, "GET %s HTTP/1.0\r\n\r\n", argv[3]);
    send(sockfd, buffer, strlen(buffer), 0);

    do {
        bzero(buffer, sizeof(buffer));
        bytes_read = recv(sockfd, buffer, sizeof(buffer), 0);
        if(bytes_read > 0) printf("%s", buffer);
    } while(bytes_read > 0);

    close(sockfd);
    return 0;
}
