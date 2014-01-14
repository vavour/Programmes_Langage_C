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
#include <time.h>

void err(char *s);

void delai ( int secondes);

int main(int argc, char** argv)
{
    if(argc<4 || (argc>4 && argc<6) || argc>6)
    {
        printf("\nSyntaxe du programme : %s ",argv[0]);
        if (argc < 6)
        {
            printf("<Adresse IP du serveur> <Port> <Commande AT>\n\n");
        }
        else
        {
            printf("<Adresse IP du serveur> <Port> <Commande AT N°1> <Commande AT N°2> <Delai>\n\n");
        }
        exit(0);
    }
    
    int p, port = 0;
    int compteur1 = strlen ( argv[2] );
    for(p=0;p<compteur1;p++)
    {
        port=port+(argv[2][p]-48)*pow(10,compteur1-(p+1));
    }
    
    struct sockaddr_in serv_addr;
    int sockfd, i, slen=sizeof(serv_addr);
    
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
    
    printf("\nAdresse du serveur distant : %s:%d\n",argv[1],port);
    
    if (argc==4)
    {
        int longueur1 = strlen ( argv[3] );
        char *commandeAT1 = malloc((( longueur1 + 2 ) * sizeof ( char )));
        int z ;
        for (z = 0 ; z < longueur1 ; z ++ )
        {
            commandeAT1[z] = argv[3][z];
        }
        commandeAT1[longueur1]='\r';
        commandeAT1[longueur1+1]='\0';
        
        printf("Commande AT : %s\n\n",commandeAT1);
        
        if (sendto(sockfd, commandeAT1, longueur1+2, 0, (struct sockaddr*)&serv_addr, slen)==-1)
        {
            err("sendto()");
        }
        
        close(sockfd);
        free(commandeAT1);
    }
    else
    {
        int longueur1 = strlen ( argv[3] );
        char *commandeAT1 = malloc((( longueur1 + 2 ) * sizeof ( char )));
        int z ;
        for (z = 0 ; z < longueur1 ; z ++ )
        {
            commandeAT1[z] = argv[3][z];
        }
        commandeAT1[longueur1]='\r';
        commandeAT1[longueur1+1]='\0';
        
        int longueur2 = strlen ( argv[4] );
        char *commandeAT2 = malloc((( longueur2 + 2 ) * sizeof ( char )));
        int n ;
        for (n = 0 ; n < longueur2 ; n ++ )
        {
            commandeAT2[n] = argv[4][n];
        }
        commandeAT2[longueur2]='\r';
        commandeAT2[longueur2+1]='\0';
        
        int delaiSecondes = 0;
        int k ;
        int compteur2 = strlen ( argv[5] );
        for(k=0;k<compteur2;k++)
        {
            delaiSecondes=delaiSecondes+(argv[5][k]-48)*pow(10,compteur2-(k+1));
        }
        
        printf("Commande AT N°1 : %s\n",commandeAT1);
        printf("Commande AT N°2 : %s\n",commandeAT2);
        printf("Delai : %d secondes\n\n",delaiSecondes);
        
        if (sendto(sockfd, commandeAT1, longueur1+2, 0, (struct sockaddr*)&serv_addr, slen)==-1)
        {
            err("sendto()");
        }
        
        delai(delaiSecondes);
        
        if (sendto(sockfd, commandeAT2, longueur2+2, 0, (struct sockaddr*)&serv_addr, slen)==-1)
        {
            err("sendto()");
        }
        
        close(sockfd);
        free(commandeAT1);
        free(commandeAT2);
    }
    return 0;
}

void err(char *s)
{
    perror(s);
    exit(1);
}

void delai ( int delaiSecondes )
{
    clock_t delai1;
    while ( clock()/CLOCKS_PER_SEC < delaiSecondes)
    {
    }
}