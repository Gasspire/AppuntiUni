/* Sample TCP client */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

typedef struct articolo{
    char nome[20];
    char taglia[5];
    int quantità;
    float prezzo;
}Articolo;


int main(int argc, char**argv)
{ int sockfd,n;
  struct sockaddr_in local_addr, dest_addr;
  char sendline[1000];
  char recvline[1000];

  if (argc != 3)
  { printf("usage:  client IP_address <Port\n");
    return 1;
  }

  sockfd=socket(AF_INET,SOCK_STREAM,0);

  memset( &dest_addr, 0, sizeof(dest_addr));
  dest_addr.sin_family = AF_INET;
  dest_addr.sin_addr.s_addr = inet_addr(argv[1]);
  dest_addr.sin_port = htons(atoi(argv[2]));

  connect(sockfd, (struct sockaddr *) &dest_addr, sizeof(dest_addr));
  int scelta;
  Articolo aux;
  for(;;){
    printf("\nCosa vuoi fare?\n1 Invio della lista\n2 Acquista\n3 Reso\n4 Fine\n");
    scanf("%d",&scelta);
    switch (scelta)
    {
    case 1:
        strcpy(sendline,"Invia lista");
        send(sockfd,sendline,strlen(sendline)+1,0);
        n = recv(sockfd,recvline,999,0);
        if(n == 0) exit(-1);
        printf("%s\n",recvline);
        break;
    case 2:
        while(1){
            strcpy(sendline,"Acquisto");
            send(sockfd,sendline,strlen(sendline)+1,0);
            printf("Quale articolo vuoi acquistare?\n");
            scanf("%s",aux.nome);
            printf("Quale taglia?\n");
            scanf("%s",aux.taglia);
            printf("Quanti ne vuoi acquistare?\n");
            scanf("%d",&aux.quantità);
            send(sockfd,&aux,sizeof(aux)+1,0);
            n = recv(sockfd,recvline,999,0);
            if(n == 0) exit(-1);
            printf("%s\n",recvline);
            printf("Vuoi continuare gli acquisti? si/no\n");
            char fine[5];
            scanf("%s",fine);
            if(strcmp(fine,"no")==0) break;
        }
        break;
    case 3:
        strcpy(sendline,"Reso");
        send(sockfd,sendline,strlen(sendline)+1,0);
        printf("Quale articolo vuoi restituire?\n");
        scanf("%s",aux.nome);
        printf("Quale taglia?\n");
        scanf("%s",aux.taglia);
        printf("Quanti ne vuoi restituire?\n");
        scanf("%d",&aux.quantità);
        send(sockfd,&aux,sizeof(aux)+1,0);
        n = recv(sockfd,recvline,999,0);
        if(n == 0) exit(-1);
        printf("%s\n",recvline);
        break;
    case 4:
        strcpy(sendline,"Fine");
        send(sockfd,sendline,strlen(sendline)+1,0);
        return 0;
        break;
    default:
        printf("Non hai effettuato una scelta valida!\n");
        break;
    }
  }
}
