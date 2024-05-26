#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>

int VerificaUsername(char *usernamericerca){
    FILE *registro = NULL;
    registro = fopen("registro.txt","r");
    if(registro == NULL){
        printf("Errore nell'apertura del file\n");
        return 404;
    }
    char buffer[100] = "inizio";
    char *username = " ";
    while (!feof(registro) && strcmp(usernamericerca,username)!=0)
    {
        fgets(buffer,100,registro);
        username = strtok(buffer,",");
    }
    fclose(registro);
    return strcmp(usernamericerca,username); //0 vuol dire che sono uguali
}
int Registrazione(char *user,char *pass){
   if(VerificaUsername(user)==0){
        printf("Errore, username gia' in uso\n");
        return 0;
   }
   FILE *registro = NULL;
   registro = fopen("registro.txt","a");
   if(registro == NULL){
        printf("Errore nell'apertura del file!\n");
        return 404;
   }
   fprintf(registro,"%s,%s\n",user,pass);
   fclose(registro);
   printf("%s,%s",user,pass);
   return 1;

}
int Verificazione(char *user,char *pass){
    FILE *registro = NULL;
    registro = fopen("registro.txt","r");
    if(registro == NULL){
        printf("Errore nell'apertura del file\n");
        return 404;
    }
    
    char buffer[100] = "inizio";
    char *username = " ";
    char *password = " ";

    while (!feof(registro))
    {
        fgets(buffer,100,registro);
        username = strtok(buffer,",");
        if(strcmp(username,user) == 0){
            password = strtok(NULL,"\n");
            if(strcmp(password,pass)==0){
                printf("Sei stato verificato!\n");
                fclose(registro);
                return 1;
            }
            else{
                printf("Username o password errata!\n");
                fclose(registro);
                return 0;
            }
        }
    }
    printf("Username o password errata!\n");
    fclose(registro);
    return 0;
}

int main(int argc, char **argv){

    int sockfd;
    struct sockaddr_in local_addr, remote_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    char buffer[100];
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
    //sendto(sockfd,msg,99,0, (struct sockaddr *) &remote_addr, sizeof(remote_addr));

    for(;;){
        printf("In attesa di un messaggio!\n");
        recvfrom(sockfd, buffer, 99, 0, (struct sockaddr *) &remote_addr, &len);
        printf("IP = %s Port = %d Msg = %s\n", inet_ntoa(remote_addr.sin_addr),ntohs(remote_addr.sin_port), buffer);
        if(strcmp(buffer,"Registrazione")==0){
            recvfrom(sockfd, buffer, 99, 0, (struct sockaddr *) &remote_addr, &len);
            buffer[99] = 0;
            printf("IP = %s Port = %d Msg = %s\n", inet_ntoa(remote_addr.sin_addr),ntohs(remote_addr.sin_port), buffer);
            char *username = strtok(buffer,",");
            char *password = strtok(NULL,"\n");
            if(Registrazione(username,password)!=1){
                char msg[100] = "Nome utente in uso o errore nell'apertura del file";
                sendto(sockfd,msg,99,0, (struct sockaddr *) &remote_addr, sizeof(remote_addr));
            }
            else{
                char msg[100] = "Ti sei registrato correttamente";
                sendto(sockfd,msg,99,0, (struct sockaddr *) &remote_addr, sizeof(remote_addr));
            }
        }
        else if(strcmp(buffer,"Login")==0){
            recvfrom(sockfd, buffer, 99, 0, (struct sockaddr *) &remote_addr, &len);
            buffer[99] = 0;
            printf("IP = %s Port = %d Msg = %s\n", inet_ntoa(remote_addr.sin_addr),ntohs(remote_addr.sin_port), buffer);
            char *username = strtok(buffer,",");
            char *password = strtok(NULL,"\n");
            if(Verificazione(username,password)==1){
                char msg[100] = "Ti sei loggato correttamente";
                sendto(sockfd,msg,99,0, (struct sockaddr *) &remote_addr, sizeof(remote_addr));
            }
            else{
                char msg[100] = "Username o password errati";
                sendto(sockfd,msg,99,0, (struct sockaddr *) &remote_addr, sizeof(remote_addr));
            }
        }

        else if(strcmp(buffer,"Stampa messaggi")==0){
            FILE *posta = NULL;
            posta = fopen(strcat(inet_ntoa(remote_addr.sin_addr),".txt"),"r+");
            if(posta == NULL){
                printf("Impossibile aprire il file!\n");
            }
            else{
                char msg[500] = " ";
                while (!feof(posta))
                {
                    fgets(msg,500,posta);
                    msg[400] = '\0';
                    sendto(sockfd,msg,strlen(msg)+1,0,(struct sockaddr *)&remote_addr,sizeof(remote_addr));
                }
                strcpy(msg,"FINE MESSAGGI\n");
                sendto(sockfd,msg,500,0,(struct sockaddr *)&remote_addr,sizeof(remote_addr));
            }
            fclose(posta);
        }
        else if(strcmp(buffer,"Scrivi messaggio")==0){
            FILE *posta = NULL;
            posta = fopen(strcat(inet_ntoa(remote_addr.sin_addr),".txt"),"a+");
            char msg[500];
            recvfrom(sockfd, msg, 500, 0, (struct sockaddr *) &remote_addr, &len);
            msg[450] = 0;
            printf("IP = %s Port = %d Msg = %s\n", inet_ntoa(remote_addr.sin_addr),ntohs(remote_addr.sin_port), msg);
            fprintf(posta,"%s",msg);
            fclose(posta);
            strcpy(msg,"Hai scritto il messaggio correttamente!");
            sendto(sockfd,msg,500,0,(struct sockaddr *)&remote_addr,sizeof(remote_addr));
        }
    }

    close(sockfd);
    return 0;
}