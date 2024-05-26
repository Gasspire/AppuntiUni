/* Sample TCP client */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>


struct prodotto{
    int id_prodotto;
    char nome_prodotto[20];
    float prezzo;
    int quantità;
};


typedef struct prodotto prodotto;





int main(int argc, char**argv)
{ int sockfd,n;
  struct sockaddr_in local_addr, dest_addr;
  char sendline[1000];
  char recvline[1000];
  char scelta[5] = "c";
  prodotto t;

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
  
  for(;;){
    send(sockfd,scelta,sizeof(scelta)+1,0);
    n = recv(sockfd,recvline,999,0);
    recvline[n] = 0;
    printf("%s",recvline);
    printf("Quale prodotto vuoi acquistare?\n");
    scanf("%d",&t.id_prodotto);
    printf("Quanti?\n");
    scanf("%d",&t.quantità);
    send(sockfd,&t,sizeof(t)+1,0);
    n = recv(sockfd,recvline,999,0);
    recvline[n] = 0;
    printf("%s",recvline);
    printf("Vuoi continuare? si(c),no(q)\n");
    scanf("%s",scelta);
    if(scelta[0] == 'q'){
      break;
    }

  }
  close(sockfd);
  return 0;
}
