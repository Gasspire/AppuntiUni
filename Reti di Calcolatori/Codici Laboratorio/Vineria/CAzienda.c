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

    int sockfd,n,scelta;
    struct sockaddr_in local_addr, dest_addr;

    char sendline[1000];
    char recvline[1000];
    char nomeazienda[50];
    char *aux =" ";
    articolo buffer;

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


    strcpy(sendline,"Azienda");
    send(sockfd,sendline,strlen(sendline),0);
    printf("Inserisci il nome della tua azienda!\n");
    scanf("%s",nomeazienda);
    send(sockfd,nomeazienda,strlen(nomeazienda)+1,0);

    n = recv(sockfd,recvline,999,0);
    recvline[n]= 0;
    printf("%s",recvline);


    for(;;){
        printf("Cosa vuoi fare?\n\n______________\n\n1 Inviare un prodotto\n2 Stampare la lista dei propri prodotti\n3 Aggiornare la lista\n4 Chiudere la connessione\n\n______________\n\n");
        scanf("%d",&scelta);
        switch (scelta)
        {
        case 1:
            strcpy(sendline,"Invio prodotto");
            send(sockfd,sendline,strlen(sendline),0);
            strcpy(buffer.nome_azienda,nomeazienda);
            printf("Inserisci il nome del vino\n");
            scanf("%s",buffer.nome_vino);
            printf("Inserisci la quantità di bottiglie a disposizione\n");
            scanf("%d",&buffer.quantita);
            printf("Inserisci il prezzo della singola bottiglia\n");
            scanf("%f",&buffer.costo);

            send(sockfd,&buffer,sizeof(buffer)+1,0);
            
            break;
        case 2:
            strcpy(sendline,"Stampa lista prodotti");
            send(sockfd,sendline,strlen(sendline)+1,0);
            //send(sockfd,nomeazienda,strlen(nomeazienda)+1,0);
            n = recv(sockfd,recvline,999,0);
            recvline[n] = 0;
            printf("%s",recvline);
            printf("Fine prodotti\n");

            break;
        case 3:
            strcpy(sendline,"Aggiorna lista");
            send(sockfd,sendline,strlen(sendline)+1,0);
            
            printf("Di quale prodotto vuoi effettuare la modifica?\n");
            scanf("%s",buffer.nome_vino);
            printf("Quale è l'attuale quantità del prodotto?\n");
            scanf("%d",&buffer.quantita);
            printf("Quanto costa attualmente il prodotto?\n");
            scanf("%f",&buffer.costo);
            
            send(sockfd,&buffer,sizeof(buffer)+1,0);
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
