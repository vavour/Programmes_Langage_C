// Client UDP
// Pierre PELE
// Paul-Darius SARMADI

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include <windows.h>
#define BUFLEN 512
//#define PORT 5556

void err(char *s)
{
    perror(s);
    exit(1);
}

//void Color(int couleurDuTexte,int couleurDeFond);

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        printf("\nSyntaxe du programme : %s ",argv[0]);
        //Color(12,3);
        printf("<Adresse IP du serveur> <Port>\n\n");
        //Color(15,0);
        exit(0);
    }

    int p, port = 0, compteur = 0;
    char charCourant;

    do
    {
        charCourant = argv[2][compteur];
        if((charCourant<48 && charCourant != '\0') || charCourant>57)
        {
        printf("\nVeuillez saisir un nombre pour le port !\n\n");
        exit(0);
        }
        compteur++;
    }
    while(charCourant !='\0');

    compteur--;
    for(p=0;p<compteur;p++)
    {
        port=port+(argv[2][p]-48)*pow(10,compteur-(p+1));
    }

    if(port < 1 || port > 65535)
    {
        printf("\nLa valeur du port est incorrecte !\n\n");
        exit(0);
    }

    printf("\nAdresse du serveur distant : %s:%d\n",argv[1],port);

    struct sockaddr_in serv_addr;
    int sockfd, i, slen=sizeof(serv_addr);
    char buf[BUFLEN];

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
        err("socket");

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_aton(argv[1], &serv_addr.sin_addr)==0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    printf("\nSaisissez EXIT afin de fermer le programme.\n");
    while(1)
    {
        printf("\nCommande AT : ");
        scanf("%[^\n]",buf);
        getchar();
        if(strcmp(buf,"EXIT") == 0)
          exit(0);

        if (sendto(sockfd, buf, BUFLEN, 0, (struct sockaddr*)&serv_addr, slen)==-1)
            err("sendto()");
    }

    close(sockfd);
    return 0;
}

/*void Color(int couleurDuTexte,int couleurDeFond) // fonction d'affichage de couleurs
{
        HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(H,couleurDeFond*16+couleurDuTexte);
}*/
