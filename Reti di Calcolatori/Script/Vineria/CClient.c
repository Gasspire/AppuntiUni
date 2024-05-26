#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

struct articolo{
    char nome_azienda[20];
    int id_articolo;
    char nome_vino[20];
    int quantita;
    float costo;
};

typedef struct articolo articolo;


int main(int argc, char**argv){ 

    int sockfd,n;
    struct sockaddr_in local_addr, dest_addr;

    char sendline[1000];
    char recvline[1000];

    if (argc != 3){
        printf("usage:  client IP_address <Port\n");
        return 1;
    }

    sockfd=socket(AF_INET,SOCK_STREAM,0);

    memset( &dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.s_addr = inet_addr(argv[1]);
    dest_addr.sin_port = htons(atoi(argv[2]));

    connect(sockfd, (struct sockaddr *) &dest_addr, sizeof(dest_addr));


    strcpy(sendline,"Cliente");
    send(sockfd,sendline,strlen(sendline),0);
    recv(sockfd,recvline,999,0);
    printf("%s",recvline);


    int scelta;


    for(;;){
        printf("Cosa vuoi fare?\n\n______________\n\n1 Stampa lista prodotti\n2 Acquista prodotto\n4 Chiudere la connessione\n\n______________\n\n");
        scanf("%d",&scelta);
        switch (scelta)
        {
        case 1:
            strcpy(sendline,"Stampa lista prodotti");
            send(sockfd,sendline,strlen(sendline)+1,0);
            n = recv(sockfd,recvline,999,0);
            recvline[n] = 0;
            printf("%s",recvline);
            printf("Fine prodotti\n");
            break;
        
        case 2:
            strcpy(sendline,"Acquista articolo");
            send(sockfd,sendline,strlen(sendline)+1,0);
            articolo t;
            printf("Quale è l'id del prodotto che vuoi acquistare?\n");
            scanf("%d",&t.id_articolo);
            printf("Quanti ne vuoi acquistare?\n");
            scanf("%d",&t.quantita);
            send(sockfd,&t,sizeof(t)+1,0);
            n = recv(sockfd,recvline,999,0);
            recvline[n] = 0;
            printf("%s",recvline);
            break;
        case 4:
            strcpy(sendline,"Chiusura");
            send(sockfd,sendline,strlen(sendline),0);
            exit(-1);
            break;
        default:
            printf("Hai inserito una scelta non valida!\n");
            break;
        }


    }




}
