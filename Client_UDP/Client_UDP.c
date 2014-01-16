#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h>

#define BUFLEN 512
#define ADRESSEIP "192.168.1.1"
#define PORT 5556
#define MESSAGE "AT*REF=101,290718208\r"

void err(char *s);

int main(int argc, char** argv)
{
    struct sockaddr_in serv_addr;
    int sockfd, i, slen=sizeof(serv_addr);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    {
        err("socket");
    }
    
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_aton(ADRESSEIP, &serv_addr.sin_addr)==0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    if (sendto(sockfd, MESSAGE, BUFLEN, 0, (struct sockaddr*)&serv_addr, slen)==-1)
    {
            err("sendto()");
    }

    close(sockfd);
    return 0;
}

void err(char *s)
{
    perror(s);
    exit(1);
}