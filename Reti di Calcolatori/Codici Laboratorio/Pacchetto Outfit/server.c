//FORMATO NOME,TAGLIA,QUANTITà,PREZZO;
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


void stampa_lista(char sendline[]){
    strcpy(sendline,"");
    FILE *db = NULL;
    db = fopen("registro.txt","r");
    if(db == NULL){
        printf("Errore nell'apertura del file\n");
        exit(0);
    }
    char line[500];
    while(fgets(line,500,db)!= NULL){
        strcat(sendline,line);
    }
    fclose(db);
}

void acquista(Articolo articolo,char sendline[]){
    Articolo token;
    char line[500];
    FILE *aux = fopen("aux.txt","w+");
    FILE *db = fopen("registro.txt","r");   
    strcpy(sendline,"Prodotto non trovato o quantità non disponibile");
    if(db == NULL || aux == NULL){
        printf("Errore nell'apertura del file\n");
        exit(0);
    }
    while(fgets(line,500,db)!= NULL){
        strcpy(token.nome,strtok(line,","));
        strcpy(token.taglia,strtok(NULL,","));
        token.quantità = atoi(strtok(NULL,","));
        token.prezzo = atof(strtok(NULL,";"));
        if(strcmp(token.nome,articolo.nome) == 0 && strcmp(token.taglia,articolo.taglia)==0 && token.quantità >= articolo.quantità){
            fprintf(aux,"%s,%s,%d,%.2f;\n",token.nome,token.taglia,(token.quantità-articolo.quantità),token.prezzo);
            strcpy(sendline,"Prodotto trovato");
        }
        else{
            fprintf(aux,"%s,%s,%d,%.2f;\n",token.nome,token.taglia,token.quantità,token.prezzo);
        }
    }
    fclose(db);
    fclose(aux);
    remove("registro.txt");
    rename("aux.txt","registro.txt");
    return;
}

void reso(Articolo articolo,char sendline[]){
    Articolo token;
    char line[500];
    FILE *aux = fopen("aux.txt","w+");
    FILE *db = fopen("registro.txt","r");   
    strcpy(sendline,"Prodotto non trovato");
    if(db == NULL || aux == NULL){
        printf("Errore nell'apertura del file\n");
        exit(0);
    }
    while(fgets(line,500,db)!= NULL){
        strcpy(token.nome,strtok(line,","));
        strcpy(token.taglia,strtok(NULL,","));
        token.quantità = atoi(strtok(NULL,","));
        token.prezzo = atof(strtok(NULL,";"));
        if(strcmp(token.nome,articolo.nome) == 0 && strcmp(token.taglia,articolo.taglia)==0){
            fprintf(aux,"%s,%s,%d,%.2f;\n",token.nome,token.taglia,(token.quantità+articolo.quantità),token.prezzo);
            strcpy(sendline,"Prodotto trovato");
        }
        else{
            fprintf(aux,"%s,%s,%d,%.2f;\n",token.nome,token.taglia,token.quantità,token.prezzo);
        }
    }
    fclose(db);
    fclose(aux);
    remove("registro.txt");
    rename("aux.txt","registro.txt");
    return;
}


int main(int argc, char**argv)
{ int sockfd,newsockfd,n;
  struct sockaddr_in local_addr,remote_addr;
  socklen_t len;
  char sendline[1000];
  char recvline[1000];
  Articolo buffer;
  
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
  local_addr.sin_addr.s_addr = htonl(INADDR_ANY); //settiamo l'indirizzo ip del server con INADDR_ANY che lo prende in automatico
  local_addr.sin_port = htons(atoi(argv[1]));
  
  if(bind(sockfd, (struct sockaddr *) &local_addr, sizeof(local_addr))<0)
  { printf("\nErrore nel binding. Errore %d \n",errno);
    return -1;
  }
  
  listen(sockfd,5); //diciamo che possiamo gestire al massimo 5 richieste di connessione contemporaneamente 

  for(;;)
  { 
    len = sizeof(remote_addr);
    newsockfd = accept(sockfd,(struct sockaddr *)&remote_addr, &len); //tutte le informazioni sulla nuova socket
    //send(newsockfd,recvline,n,0);  
    if (fork() == 0) 
    { 
    close(sockfd);
	  for(;;){
        n = recv(newsockfd,recvline,999,0);
        if(n==0) return 0;
        recvline[n] = 0;
        printf("\nPid=%d: received from %s:%d the following: %s\n",getpid(),inet_ntoa(remote_addr.sin_addr), ntohs(remote_addr.sin_port), recvline );
        if(strcmp(recvline,"Invia lista")==0){
            stampa_lista(sendline);
            if(send(newsockfd,sendline,strlen(sendline)+1,0)==-1) exit(-1);
        }
        else if(strcmp(recvline,"Acquisto")==0){
            recv(newsockfd, &buffer, sizeof(buffer)+1, 0);
            acquista(buffer,sendline);
            if(send(newsockfd,sendline,strlen(sendline)+1,0)==-1) exit(-1);
        }
        else if(strcmp(recvline,"Reso")==0){
            recv(newsockfd, &buffer, sizeof(buffer)+1, 0);
            reso(buffer,sendline);
            if(send(newsockfd,sendline,strlen(sendline)+1,0)==-1) exit(-1);
        }
        else if(strcmp(recvline,"Fine")==0){
            return 0;
        }

      }   
      return 0;      
    }
	else
      close(newsockfd); 
  }
}
