#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>


int trovanum(char nomefile[]){
    char line[500];
    FILE *db = NULL;
    int numero = 1;
    db = fopen(nomefile,"r");
    if(db == NULL) {
        return 1;
    }
    while(fgets(line,500,db)!=NULL) numero++;
    fclose(db);
    return numero;
}

void invio(char *messaggio,char ip[], char sendline[]){
    memset(sendline,0,500);
    FILE *db = NULL;
    char nomefile[50];
    strcpy(nomefile,strcat(ip,".txt"));
    int num_mesg = trovanum(nomefile);

    db = fopen(nomefile,"a+");
    if(db == NULL) {
        strcpy(sendline,"Qualcosa è andato storto\n");
        return;
    }
    fprintf(db,"%d,%s;\n",num_mesg,messaggio);
    fclose(db);
    strcpy(sendline,"Tutto è andato a buon fine\n");
}

void stampamessaggi(char ip[], char sendline[]){
    memset(sendline,0,500);
    FILE *db =NULL;
    char aux[500];
    db = fopen(strcat(ip,".txt"),"r");
    if(db == NULL) {
        strcpy(sendline,"Qualcosa è andato storto\n");
        return;
    }
    while (fgets(aux,500,db)!=NULL)
    {
        strcat(sendline,aux);
    }

    fclose(db);
    return;
}

void modifica_messaggio(int numero_messaggio,char *messaggio,char ip[], char sendline[]){
    memset(sendline,0,500);
    FILE *db = NULL;
    FILE *aux = NULL;
    char nomefile[50];
    char line[500];
    char token[500];
    strcpy(nomefile,strcat(ip,".txt"));
    db = fopen(nomefile,"r");
    aux = fopen("aux.txt","w+");
    if(db == NULL || aux == NULL){
        strcpy(sendline,"C'è stato un errore nell'apertura dei file\n");
        return;
    }
    strcpy(sendline,"");
    while (fgets(token,500,db)!=NULL)
    {
        strcpy(line,token);
        int num = atoi(strtok(token,","));
        if(num != numero_messaggio){
            fprintf(aux,"%s",line);
        }
        else{
            fprintf(aux,"%d,%s;\n",num,messaggio);
        }
    }
    fclose(db);
    fclose(aux);
    remove(nomefile);
    rename("aux.txt",nomefile);
    strcpy(sendline,"Tutto è andato a buon fine\n");
    return;
}

void cancellamessaggio(int numero_messaggio,char ip[], char sendline[]){
    char nomefile[50];
    char line[500];
    char *messaggio;
    strcpy(nomefile,strcat(ip,".txt"));
    FILE *db = NULL;
    FILE *aux = NULL;
    int contatore = 1;
    db = fopen(nomefile,"r");
    aux = fopen("aux.txt","w+");
    if(db == NULL || aux == NULL){
        strcpy(sendline,"C'è stato un errore nell'apertura dei file\n");
        return;
    }
    while (fgets(line,500,db)!=NULL)
    {
        int num = atoi(strtok(line,","));
        messaggio = strtok(NULL,";") ;
        if(num != numero_messaggio){
            fprintf(aux,"%d,%s;\n",contatore,messaggio);
            contatore++;
        }
    }
    fclose(db);
    fclose(aux);
    remove(nomefile);
    rename("aux.txt",nomefile);
    strcpy(sendline,"Tutto è andato a buon fine\n");
    return;
}


int main(int argc, char **argv){

    int sockfd,n;
    struct sockaddr_in local_addr, remote_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    char recvline[500];
    char sendline[500];

    if(argc < 2){
        printf("Errore. Devi inserire il numero di porta\n");
        return -1;
    }

    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0){
        printf("Errore nell'apertura di socket\n");
        return -1;
    }

    memset(&local_addr, 0, len);
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(atoi(argv[1]));

    if(bind(sockfd, (struct sockaddr *) &local_addr, len) < 0){
        printf("Errore nell'apertura di bind()\n Utilizza un altro valore\n");
        return -1;
    }
    //recvfrom(sockfd, buffer, 99, 0, (struct sockaddr *) &remote_addr, &len);
    //printf("IP = %s Port = %d Msg = %s\n", inet_ntoa(remote_addr.sin_addr),ntohs(remote_addr.sin_port), buffer);
    
    char action[50];
    
    for(;;){
        n = recvfrom(sockfd, recvline, 499, 0, (struct sockaddr *) &remote_addr, &len);
        recvline[n] = 0;
        strcpy(action,strtok(recvline,","));
        if(strcmp(action,"Invio")==0){
            char *messaggio;
            messaggio = strtok(NULL,";");
            invio(messaggio,inet_ntoa(remote_addr.sin_addr),sendline);
            sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr*)&remote_addr,sizeof(remote_addr));
        }
        else if(strcmp(action,"Stampa")==0){
            stampamessaggi(inet_ntoa(remote_addr.sin_addr),sendline);
            sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr*)&remote_addr,sizeof(remote_addr));
        }
        else if(strcmp(action,"Modifica")==0){
            int numero_messaggio = atoi(strtok(NULL,","));
            char *messaggio = strtok(NULL,";");
            modifica_messaggio(numero_messaggio,messaggio,inet_ntoa(remote_addr.sin_addr),sendline);
            sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr*)&remote_addr,sizeof(remote_addr));
        }
        else if(strcmp(action,"Cancella")==0){
            int numero_messaggio = atoi(strtok(NULL,";"));
            cancellamessaggio(numero_messaggio,inet_ntoa(remote_addr.sin_addr),sendline);
            sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr*)&remote_addr,sizeof(remote_addr));
        }

    }

    close(sockfd);
    return 0;
}