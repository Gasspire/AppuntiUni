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


int main(int argc, char**argv)
{ 
    int sockfd,newsockfd,n;
    struct sockaddr_in local_addr,remote_addr;
    socklen_t len;
    char mesg[1000];
    char line[1000];
    char aux[1000];
    char nomeazienda[20];
    int idarticolo = 1;
    int IsAClient = 0;
    articolo buffer;
    FILE *db = NULL;

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
    
    if(bind(sockfd, (struct sockaddr *) &local_addr, sizeof(local_addr))<0){ 
        printf("\nErrore nel binding. Errore %d \n",errno);
        return -1;
    }
    
    listen(sockfd,5); 

    for(;;)
    { 
        len = sizeof(remote_addr);
        newsockfd = accept(sockfd,(struct sockaddr *)&remote_addr, &len); 
        
        if (fork() == 0){ 
            close(sockfd);
            recv(newsockfd,mesg,999,0); //Prima recv, aspettiamo che il client si identifichi come azienda o come cliente 
            if(strcmp(mesg,"Cliente")==0){
                IsAClient = 1;
                strcpy(mesg,"Ti sei identificato come Cliente!\n");
                send(newsockfd,mesg,strlen(mesg),0);
            }
            else if(strcmp(mesg,"Azienda")==0){
                recv(newsockfd,nomeazienda,19,0);
                strcpy(mesg,"Ti sei identificato come Azienda\n");
                send(newsockfd,mesg,strlen(mesg),0);
            }

            for(;;){
                n = recv(newsockfd,mesg,999,0);
                mesg[n] = 0;
                printf("Ho ricevuto %s\n",mesg);
                
                
                if(strcmp(mesg,"Invio prodotto")==0){
                    recv(newsockfd,&buffer,sizeof(articolo),0);
                    buffer.id_articolo = idarticolo;
                    idarticolo++;
                    db = fopen("registro.txt","a+");
                    if (db==NULL) exit(-1);
                    fprintf(db,"%s,%d,%s,%d,%.2f;\n",buffer.nome_azienda,buffer.id_articolo,buffer.nome_vino,buffer.quantita,buffer.costo);
                    fclose(db);
                }
                
                
                else if(strcmp(mesg,"Stampa lista prodotti")==0){
                    db = fopen("registro.txt","r+");
                    if(db==NULL) exit(-1);
                    if(IsAClient == 1){
                      strcpy(mesg,"");
                        while(fgets(line,999,db) != NULL){
                            strcat(mesg,line);
                        }
                    }
                    else{
                        //recv(newsockfd,mesg,999,0);
                        //strcpy(nomeazienda,mesg);
                        strcpy(mesg,"");
                        while(fgets(line,999,db) != NULL){
                            strcpy(aux,line);
                            strcpy(aux,strtok(aux,","));
                            if(strcmp(aux,nomeazienda)==0){
                                strcat(mesg,line);
                            }
                        }
                    }
                    send(newsockfd,mesg,strlen(mesg)+1,0);
                    fclose(db);
                }
                
                
                else if(strcmp(mesg,"Aggiorna lista")==0){
                    if(IsAClient == 1) continue;
                    recv(newsockfd,&buffer,sizeof(articolo),0);

                    db = fopen("registro.txt","r+");
                    FILE *fp = NULL;
                    articolo t;
                    fp = fopen("aux.txt","w");

                    if(db == NULL || fp == NULL) exit(-1);

                    while (fgets(line,500,db) != NULL)
                    {
                        strcpy(aux,strtok(line,","));
                        strcpy(t.nome_azienda,aux);
                        strcpy(aux,strtok(NULL,","));
                        t.id_articolo = atoi(aux);
                        strcpy(aux,strtok(NULL,","));
                        strcpy(t.nome_vino,aux);
                        if(strcmp(aux,buffer.nome_vino)==0){
                            t.costo = buffer.costo;
                            t.quantita = buffer.quantita;
                        }
                        else{
                            strcpy(aux,strtok(NULL,","));
                            t.quantita = atoi(aux);
                            strcpy(aux,strtok(NULL,","));
                            t.costo = atof(aux);
                        }
                        fprintf(fp,"%s,%d,%s,%d,%.2f;\n",t.nome_azienda,t.id_articolo,t.nome_vino,t.quantita,t.costo);
                    }
                    fclose(db);
                    fclose(fp);
                    remove("registro.txt");
                    rename("aux.txt","registro.txt");
                }

                else if(strcmp(mesg,"Acquista articolo")==0){
                    recv(newsockfd,&buffer,sizeof(articolo),0);

                    db = fopen("registro.txt","r+");
                    FILE *fp = NULL;
                    articolo t;
                    fp = fopen("aux.txt","w");

                    if(db == NULL || fp == NULL) exit(-1);
                    
                    while (fgets(line,500,db) != NULL)
                    {
                        strcpy(aux,strtok(line,","));
                        strcpy(t.nome_azienda,aux);
                        strcpy(aux,strtok(NULL,","));
                        t.id_articolo = atoi(aux);
                        strcpy(aux,strtok(NULL,","));
                        strcpy(t.nome_vino,aux);
                        strcpy(aux,strtok(NULL,","));
                        t.quantita = atoi(aux);
                        strcpy(aux,strtok(NULL,","));
                        t.costo = atof(aux);
                        strcpy(mesg,"La merce non è stata trovata!");
                        if(t.id_articolo == buffer.id_articolo){
                            if((t.quantita - buffer.quantita)>=0){
                                t.quantita = t.quantita-buffer.quantita;
                                strcpy(mesg,"La transazione è andata a buon fine!\n");
                            }
                            else{
                                strcpy(mesg,"Merce insufficiente!\n");
                            }
                        }

                        send(newsockfd,mesg,strlen(mesg)+1,0);
                        fprintf(fp,"%s,%d,%s,%d,%.2f;\n",t.nome_azienda,t.id_articolo,t.nome_vino,t.quantita,t.costo);
                    }
                    fclose(db);
                    fclose(fp);
                    remove("registro.txt");
                    rename("aux.txt","registro.txt");
                }
                
                if(strcmp(mesg,"Chiusura")==0) return 0;
            }
            return 0;      
        }
        else close(newsockfd); 
    }
}