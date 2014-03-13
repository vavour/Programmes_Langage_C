// Pierre Pele
// AmourDeMai FENG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <memory.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")

void err ( const char *s );

void delai ( int secondes );

int main ( int argc, char** argv ) {

    if ( argc<4 || (argc>4 && argc<6) || argc>6 ) {

        printf ( "\nSyntaxe du programme : %s ",argv[0] );
        printf ( "<Adresse IP du serveur> <Port> " );
        printf ( "\033[%sm","31" );
        printf ( "<Commande AT>\n" );
        printf ( "\033[%sm","0" );
        printf ( "\nSyntaxe alternative : %s ",argv[0] );
        printf ( "<Adresse IP du serveur> <Port> " );
        printf ( "\033[%sm","31" );
        printf ( "<Commande AT N¡ã1> " );
        printf ( "\033[%sm","0" );
        printf ( "<Delai> " );
        printf ( "\033[%sm","31" );
        printf ( "<Commande AT N¡ã2>\n\n" );
        printf ( "\033[%sm","0" );
        exit ( 0 );
    }
 
    FILE *fichier = NULL;
    char nomDuFichier[] = "Notes.txt";
    fichier = fopen ( nomDuFichier,"a" );

    time_t now = time ( NULL );
    struct tm tm_now = *localtime ( &now );
    char s_now[sizeof "JJ/MM/AAAA HH:MM:SS"];
    strftime ( s_now, sizeof s_now, "%d/%m/%Y %H:%M:%S", &tm_now );
    fprintf ( fichier,"\n\n%s\n",s_now );
    if (fichier == NULL) {
    	
        printf ( "\033[%sm","31" );
        printf ( "Ouverture du fichier %s impossible !\n",nomDuFichier );
        printf ( "\033[%sm","0" );
    }
    
    int p, port = 0;
    int compteur1 = strlen ( argv[2] );
    for ( p=0; p<compteur1; p++ ) {
    	
        if (argv[2][p]-'0' >= 0 && argv[2][p]-'0' <= 9) {

            port = port + (argv[2][p]-'0') * pow(10,compteur1-(p+1));
        }

        else{
        	
            printf ( "\033[%sm","31" );
            printf ( "\nERREUR : " );
            printf ( "\033[%sm","0" );
            printf ( "Port incorrect\n\n" );
            if ( fichier != NULL ) {
                fprintf(fichier,"ERREUR : Port incorrect\n");
            }
            exit ( 0 );
        }
    }
    
    WSAData wsaData;
  	WSAStartup(MAKEWORD(2, 2), &wsaData);
  	
    struct sockaddr_in serv_addr;
    int sockfd, slen = sizeof(serv_addr);
    if ( ( sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) ) ==-1 ) {
        err("socket");
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
  
	serv_addr.sin_addr.s_addr = inet_addr ( argv[1] );

    printf ( "\nAdresse du serveur distant : %s:%d\n", argv[1], port );
    if ( fichier != NULL ) {
        fprintf ( fichier, "Adresse du serveur distant : %s:%d\n", argv[1], port );
    }
 
    if ( argc==4 ) {
        int longueur1 = strlen ( argv[3] );
        if ( longueur1 >= 1024 ){
            printf ( "\033[%sm", "31" );
            printf ( "\nERREUR : " );
            printf ( "\033[%sm", "0" );
            printf ( "Taille du paquet incorrecte\n\n" );
            if ( fichier != NULL ){
                fprintf ( fichier, "ERREUR : Taille du paquet incorrecte\n" );
            }
            exit(0);
        }
        char *commandeAT1 = (char *)malloc((( longueur1 + 2 ) * sizeof ( char )));
        int z ;
        for (z = 0 ; z < longueur1 ; z ++ )
        {
            commandeAT1[z] = argv[3][z];
        }
        commandeAT1[longueur1] = '\r';
        commandeAT1[longueur1+1] = '\0';
        printf ( "Commande AT : " );
        printf ( "\033[%sm","31" );
        printf ( "%s\n\n",commandeAT1 );
        printf ( "\033[%sm","0" );
        if ( fichier != NULL ) {
            fprintf ( fichier,"Commande AT : %s\n",commandeAT1 );
        }
        
        int i;
        if ( ( i = sendto(sockfd, commandeAT1, longueur1+2, 0, (struct sockaddr*)&serv_addr, slen) )==-1) {

            if (fichier != NULL)
            {
                fprintf ( fichier,"ERREUR : Envoi du paquet UDP impossible\n" );
            }
            err ( "sendto" );
        }
        
        printf ( "on a envoye%d infos an drone\n\n", i );
        free ( commandeAT1 );
    }
    
    else{
    	
        int longueur1 = strlen ( argv[3] );
        if ( longueur1 >= 1024 ) {
            printf ( "\033[%sm","31" );
            printf ( "ERREUR : " );
            printf ( "\033[%sm","0" );
            printf ( "Taille du paquet incorrecte\n\n" );
            if ( fichier != NULL ){
                fprintf ( fichier, "ERREUR : Taille du paquet incorrecte\n" );
            }
            exit(0);
        }
        char *commandeAT1 = (char *)malloc((( longueur1 + 2 ) * sizeof ( char )));
        int z ;
        for (z = 0 ; z < longueur1 ; z ++ ) {
            commandeAT1[z] = argv[3][z];
        }
        commandeAT1[longueur1] ='\r';
        commandeAT1[longueur1+1] ='\0';
        int longueur2 = strlen ( argv[5] );
        if ( longueur2 >= 1024 ) {
            printf ( "\033[%sm","31" );
            printf ( "ERREUR : " );
            printf ( "\033[%sm","0" );
            printf ( "Taille du paquet incorrecte\n\n" );
            if ( fichier != NULL ) {
                fprintf ( fichier, "ERREUR : Taille du paquet incorrecte\n" );
            }
            exit ( 0 );
        }
        char *commandeAT2 = (char *) malloc((( longueur2 + 2 ) * sizeof ( char )));
        int n ;
        for (n = 0 ; n < longueur2 ; n ++ ) {
            commandeAT2[n] = argv[5][n];
        }
        commandeAT2[longueur2] = '\r';
        commandeAT2[longueur2+1] = '\0';
        int delaiSecondes = 0;
        int k ;
        int compteur2 = strlen ( argv[4] );
        for( k=0; k < compteur2; k++) {
            if ( argv[4][k]-'0' >= 0 && argv[4][k]-'0' <= 9 ) {
                delaiSecondes = delaiSecondes + ( argv[4][k]-'0' ) * pow(10,compteur2-(k+1));
            }
            else {
                printf ( "\033[%sm","31" );
                printf ( "\nERREUR : " );
                printf ( "\033[%sm","0" );
                printf ( "Delai incorrect\n\n" );
                if ( fichier != NULL ) {
                    fprintf ( fichier, "ERREUR : Delai incorrect\n" );
                }
                exit ( 0 );
            }
        }
        printf ( "Commande AT N¡ã1 :" );
        printf ( "\033[%sm","31" );
        printf ( " %s\n",commandeAT1 );
        printf ( "\033[%sm","0" );
        printf ( "Commande AT N¡ã2 :" );
        printf ( "\033[%sm","31" );
        printf ( " %s\n",commandeAT2 );
        printf ( "\033[%sm","0" );
        printf ( "Delai : %d secondes\n\n", delaiSecondes );
        if (fichier != NULL) {
            fprintf ( fichier, "Commande AT N¡ã1 : %s\n",commandeAT1 );
            fprintf ( fichier, "Commande AT N¡ã2 : %s\n",commandeAT2 );
            fprintf ( fichier, "Delai : %d secondes\n",delaiSecondes );
        }
        if ( sendto(sockfd, commandeAT1, longueur1+2, 0, (struct sockaddr*)&serv_addr, slen)==-1 ) {
            if ( fichier != NULL ) {
                fprintf ( fichier, "ERREUR : Envoi du paquet UDP impossible\n" );
            }
            err ( "sendto" );
        }
        delai ( delaiSecondes );
        if ( sendto ( sockfd, commandeAT2, longueur2+2, 0, (struct sockaddr*)&serv_addr, slen)==-1 ) {
            if ( fichier != NULL ) {
                fprintf ( fichier, "ERREUR : Envoi du paquet UDP impossible\n" );
            }
            err ( "sendto" );
        }
        free(commandeAT1);
        free(commandeAT2);
    }
    
    WSACleanup;
    closesocket(sockfd);
    fclose(fichier);
    return 0;
}

void err ( const char *s ) {

    perror ( s );
    exit ( 1 );
}

void delai ( int delaiSecondes ) {

    while ( clock()/CLOCKS_PER_SEC < delaiSecondes)
    {
    }
}

/*static void init(void) {

#ifdef WIN32
  WSADATA wsa;
  int err = WSAStartup(MAKEWORD(2, 2), &wsa);
  if ( err < 0 ) {
      puts("WSAStartup failed !");
      exit(EXIT_FAILURE);
    }
#endif
}

static void end(void)
{
#ifdef WIN32
  WSACleanup();
#endif
}
*/
