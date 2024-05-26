/* Sample TCP server */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>


#define db_prodotti "registro.txt"

struct prodotto{
    int id_prodotto;
    char nome_prodotto[20];
    float prezzo;
    int quantità;
};


typedef struct prodotto prodotto;


void inviaprodotti(int newsockfd){
    FILE *db = NULL;
    char line[500];
    char mesg[1000] = "";
    db = fopen(db_prodotti,"r+");
    if(db ==NULL){ 
        fprintf(stderr,"Errore nell'apertura del file\n");
        return;
    }
    while(fgets(line,500,db)!= NULL){
        strcat(mesg,line);
    }
    send(newsockfd,mesg,strlen(mesg)+1,0);
    fclose(db);
    return;
}   

void acquisto(int newsockfd){
    prodotto t;
    prodotto aux;
    char line[500];
    char mesg[500];
    recv(newsockfd,&t,sizeof(prodotto)+1,0);
    FILE *db = NULL;
    FILE *fp = NULL;
    db = fopen(db_prodotti,"r+");
    fp = fopen("aux.txt","w");

    while(fgets(line,500,db)!= NULL){
        aux.id_prodotto = atoi(strtok(line,","));
        strcpy(aux.nome_prodotto,strtok(NULL,","));
        aux.prezzo = atof(strtok(NULL,","));
        aux.quantità = atoi(strtok(NULL,"\n"));
        if(aux.id_prodotto == t.id_prodotto){
            if(aux.quantità >= t.quantità){
                aux.quantità = aux.quantità-t.quantità;
                strcpy(mesg,"Prodotto erogato correttamente!\n");
            }
            else{
                strcpy(mesg,"Quantità non disponibile\n");
            }
        }
        fprintf(fp,"%d,%s,%.2f,%d \n",aux.id_prodotto,aux.nome_prodotto,aux.prezzo,aux.quantità);
    }
    if(strcmp(mesg,"Prodotto erogato correttamente!\n")!=0 && strcmp(mesg,"Quantità non disponibile\n")!=0){
        strcpy(mesg,"Prodotto non trovato\n");
    }

    send(newsockfd,mesg,strlen(mesg)+1,0);
    fclose(db);
    fclose(fp);
    remove(db_prodotti);
    rename("aux.txt",db_prodotti);
    return;
}


int main(int argc, char**argv)
{ int sockfd,newsockfd,n;
  struct sockaddr_in local_addr,remote_addr;
  socklen_t len;
  char mesg[1000];



  if(argc < 2)
  {	 printf("Use: server listeing_PORT");
	 return 0;
  }	 
  
  if((sockfd=socket(AF_INET,SOCK_STREAM,0)) <0)
  { printf("\nErrore nell'apertura del socket");
    return -1;
  }
  memset((char *) &local_addr,0,sizeof(local_addr));
  local_addr.sin_family = AF_INET;
  local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  local_addr.sin_port = htons(atoi(argv[1]));
  
  if(bind(sockfd, (struct sockaddr *) &local_addr, sizeof(local_addr))<0)
  { printf("\nErrore nel binding. Errore %d \n",errno);
    return -1;
  }
  
  listen(sockfd,5);

  for(;;)
  { 
    len = sizeof(remote_addr);
    newsockfd = accept(sockfd,(struct sockaddr *)&remote_addr, &len); //tutte le informazioni sulla nuova socket
	
    if (fork() == 0) 
    { 
    close(sockfd);
	  for(;;){
        n = recv(newsockfd,mesg,999,0);
        mesg[n] = 0;
        if(strcmp(mesg,"c") == 0){
            inviaprodotti(newsockfd);
            acquisto(newsockfd);
        }
        else if(strcmp(mesg,"q")==0){
            return 0;
        }
      }   
      return 0;      
    }
	else
      close(newsockfd);  //se invece è il processo padre, allora chiuderà la nuova socket dato che non la gestirà lui
  }
}
