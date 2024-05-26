#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#define registro "registro.txt"

typedef struct Farmaco{
  int id;
  char nome_farmaco[20];
  int quantita;
} Farmaco;

void invialista(int sockfd){
  FILE* db = NULL;
  char line[500] = "";
  char aux[500] = "";

  db = fopen(registro,"r+");
  if(db == NULL) return;

  while(fgets(aux,500,db)!=NULL){
    strcat(line,aux);
  }
  send(sockfd,line,strlen(line)+1,0);
  fclose(db);
  return;
}

void acquistap(char recvline[],int sockfd){
  FILE *db = NULL;
  FILE *copia = NULL;
  Farmaco t;
  Farmaco farmaco;

  farmaco.id = atoi(strtok(recvline,","));
  farmaco.quantita = atoi(strtok(NULL,";"));

  char line[500];
  db = fopen(registro,"r+");
  copia = fopen("copia.txt","w+");
  if(db == NULL || copia == NULL) return;
  
  char mesg[500];
  strcpy(mesg,"Farmaco non trovato!");
  while(fgets(line,500,db)!= NULL){
    t.id = atoi(strtok(line,","));
    strcpy(t.nome_farmaco,strtok(NULL,","));
    t.quantita = atoi(strtok(NULL,";"));

    if(t.id == farmaco.id){
      if(t.quantita >= farmaco.quantita){
        t.quantita = t.quantita - farmaco.quantita;
        strcpy(mesg,"Farmaco comprato!");
      }
      else{
        strcpy(mesg,"Errore nell'acquisto del farmaco!, quantità non disponibile!");
      }
    }
    fprintf(copia,"%d,%s,%d;\n",t.id,t.nome_farmaco,t.quantita);
  }
  fclose(copia);
  fclose(db);
  remove(registro);
  rename("copia.txt",registro);
  send(sockfd,mesg,strlen(mesg)+1,0);
}
void acquistar(char recvline[],int sockfd){
  FILE *db = NULL;
  FILE *copia = NULL;
  FILE *spedizioni = NULL;
  Farmaco t;
  Farmaco farmaco;
  char indirizzo[20];
  char line[500];

  farmaco.id = atoi(strtok(recvline,","));
  farmaco.quantita = atoi(strtok(NULL,","));
  strcpy(indirizzo,strtok(NULL,";"));


  db = fopen(registro,"r+");
  copia = fopen("copia.txt","w+");
  spedizioni = fopen("spedizioni.txt","a+");

  if(db == NULL || copia == NULL || spedizioni == NULL) return;
  
  char mesg[500];
  strcpy(mesg,"Farmaco non trovato!");
  while(fgets(line,500,db)!= NULL){
    t.id = atoi(strtok(line,","));
    strcpy(t.nome_farmaco,strtok(NULL,","));
    t.quantita = atoi(strtok(NULL,";"));

    if(t.id == farmaco.id){
      if(t.quantita >= farmaco.quantita){
        strcpy(farmaco.nome_farmaco,t.nome_farmaco);
        t.quantita = t.quantita - farmaco.quantita;
        fprintf(spedizioni,"%d,%s,%d,%s;\n",farmaco.id,farmaco.nome_farmaco,farmaco.quantita,indirizzo);
        strcpy(mesg,"Farmaco comprato!");
      }
      else{
        strcpy(mesg,"Errore nell'acquisto del farmaco!, quantità non disponibile!");
      }
    }
    fprintf(copia,"%d,%s,%d;\n",t.id,t.nome_farmaco,t.quantita);
  }
  fclose(copia);
  fclose(db);
  fclose(spedizioni);
  remove(registro);
  rename("copia.txt",registro);
  send(sockfd,mesg,strlen(mesg)+1,0);
}




int main(int argc, char**argv){ 
  int sockfd,newsockfd,n;
  struct sockaddr_in6 local_addr,remote_addr;
  socklen_t len = sizeof(struct sockaddr_in6);
  
  
  char recvline[1000];
  char sendline[1000];
  char ipv6_addr[INET6_ADDRSTRLEN];

  int IsRemote;

  if(argc < 2)
  {	 printf("Use: server listeing_PORT");
	 return 0;
  }	 
  
  if((sockfd=socket(AF_INET6,SOCK_STREAM,0)) <0)
  { printf("\nErrore nell'apertura del socket");
    return -1;
  }
  
  memset(&local_addr,0,len);
  local_addr.sin6_family = AF_INET6;
  inet_pton(AF_INET6, "::", &(local_addr.sin6_addr));
  local_addr.sin6_port = htons(atoi(argv[1]));
  
  if(bind(sockfd, (struct sockaddr *) &local_addr, len)<0)
  { printf("\nErrore nel binding. Errore %d \n",errno);
    return -1;
  }
  
  listen(sockfd,5); //diciamo che possiamo gestire al massimo 5 richieste di connessione contemporaneamente 

  for(;;)
  { 
    len = sizeof(remote_addr);
    newsockfd = accept(sockfd,(struct sockaddr *)&remote_addr, &len); //tutte le informazioni sulla nuova socket
	  
    if (fork() == 0){ 
      n = recv(newsockfd,recvline,999,0);
      recvline[n] = 0;
      if(strcmp(recvline,"Remoto")==0)IsRemote = 1;
      else IsRemote = 0;



      close(sockfd);//se è il figlio, chiudiamo la socket copia del padre 
	    for(;;){
        invialista(newsockfd);
        recv(newsockfd,recvline,999,0);
        if(IsRemote == 0) acquistap(recvline,newsockfd);
        if(IsRemote == 1) acquistar(recvline,newsockfd);

        




      
      
      
      }   
      return 0;      
    }
	else
      close(newsockfd);  //se invece è il processo padre, allora chiuderà la nuova socket dato che non la gestirà lui
  }
}
