#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>




int main(int argc, char**argv)
{ int sockfd,n;
  struct sockaddr_in6 local_addr, dest_addr;
  char sendline[1000];
  char recvline[1000];
  char id[10];
  char quantita[10];
  char ipv6_addr[INET6_ADDRSTRLEN];
  socklen_t len = sizeof(struct sockaddr_in6);
  


  if (argc != 3)
  { printf("usage:  client IP_address <Port\n");
    return 1;
  }

  sockfd=socket(AF_INET6,SOCK_STREAM,0);

  memset( &dest_addr, 0, len);
  dest_addr.sin6_family = AF_INET6;
  inet_pton(AF_INET6, argv[1], &(dest_addr.sin6_addr));
  dest_addr.sin6_port = htons(atoi(argv[2]));

  connect(sockfd, (struct sockaddr *) &dest_addr, sizeof(dest_addr));
  
  strcpy(sendline,"Presente");
  send(sockfd,sendline,strlen(sendline)+1,0);


  for(;;){
    n = recv(sockfd,recvline,999,0);
    recvline[n] = 0;
    printf("%s\n",recvline);
    
    
    printf("Inserisci l'id del prodotto che vuoi acquistare!\n");
    scanf("%s",id);
    printf("Quanti ne vuoi acquistare?\n");
    scanf("%s",quantita);

    strcpy(sendline,"");
    strcat(sendline,id);
    strcat(sendline,",");
    strcat(sendline,quantita);
    strcat(sendline,";");
    send(sockfd,sendline,strlen(sendline)+1,0);

    n = recv(sockfd,recvline,999,0);
    recvline[n] = 0;
    printf("%s\n",recvline);

    printf("Se vuoi smettere di acquistare premi q, qualunque tasto altrimenti\n");
    scanf("%s",recvline);
    if(strcmp(recvline,"q")==0) break;

  }
}
