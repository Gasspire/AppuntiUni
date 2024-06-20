/* Sample TCP client */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

int main(int argc, char **argv)
{
    int sockfd, n;
    struct sockaddr_in6 local_addr, dest_addr;
    char sendline[1000];
    char recvline[1000];

    if (argc != 3)
    {
        printf("usage:  client IP_address <Port\n");
        return 1;
    }

    sockfd = socket(AF_INET6, SOCK_STREAM, 0);

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin6_family = AF_INET6;
    inet_pton(AF_INET6, (argv[1]), &dest_addr.sin6_addr);
    // dest_addr.sin6_addr.s_addr = inet_addr(argv[1]);
    dest_addr.sin6_port = htons(atoi(argv[2]));

    connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    send(sockfd,"Presenza",strlen("Presenza")+1,0);
    int scelta;
    int id;
    int quantita;
    for (;;)
    {
        /*send(sockfd, sendline, strlen(sendline), 0); // non specifichiamo nulla riguardo all'indirizzo di spedizion perché la connessione è già spedita
        n = recv(sockfd, recvline, 999, 0);          // n contiene il numero di byte ricevuti
        recvline[n] = 0;
        printf("\nPid=%d: received from %s:%d the following: %s\n", getpid(),inet_ntoa(dest_addr.sin_addr), ntohs(dest_addr.sin_port), recvline);*/

        printf("Cosa vuoi fare?\n1 Stampa Lista\n2 Acquista\n3 Fine\n");
        scanf("%d",&scelta);
        switch (scelta)
        {
        case 1:
            strcpy(sendline,"Stampa");
            if(send(sockfd,sendline,strlen(sendline)+1,0)<0) printf("Errore nella send\n");
            n = recv(sockfd,recvline,999,0);
            recvline[n] = 0;
            if(n < 0) printf("Errore nella recv\n");
            printf("%s\n",recvline);
            break;
        case 2:
            printf("Inserisci l'id del farmaco da acquistare: ");
            scanf("%d",&id);
            printf("\nInserisci la quantita che vuoi acquistare: ");
            scanf("%d",&quantita);
            printf("Stai acquistando %d del prodotto %d\n",quantita,id);
            memset(sendline,0,999);
            sprintf(sendline,"Acquisto,%d,%d;",id,quantita);
            send(sockfd,sendline,strlen(sendline)+1,0);
            n = recv(sockfd,recvline,999,0);
            recvline[n] = 0;
            printf("%s\n",recvline);
            break;
        case 3:
            printf("Chiusura in corso...\n");
            send(sockfd,"Chiusura",strlen("Chiusura")+1,0);
            close(sockfd);
            return 0;
        default:
            printf("Inserisci una scelta valida...\n\n");
            break;
        }
    
    }
}
