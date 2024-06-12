#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<math.h>
#include<arpa/inet.h>

typedef struct client{
    char username[50];
    long int token;
    char ipv6_addr[50];
    int porta;
}Client;


int ricercausername(char *username){
    FILE *db = NULL;
    char line[500];
    char *token;
    db = fopen("database.txt","r");
    if(db == NULL){ //il file o non esiste o è vuoto, in questo caso quello che facciamo è ritornare 1 dato che se è vuoto possiamo scrivere tranquillamente
        return 1;
    }
    
    while (fgets(line,500,db)!=NULL)
    {
        token = strtok(line,",");
        if(strcmp(username,token)==0) return 0;
    }
    fclose(db);
    return 1;
}

int generatoken() {
    srand(time(NULL));
    return ((rand()%(int) pow(2,32)+1));
}



void registrazione(char *username,char* password,int porta,char sendline[]){
    int token = generatoken();
    if(ricercausername(username)==0){
        strcpy(sendline,"Username già utilizzato\n");
        return;
    }
    FILE *db = NULL;
    db = fopen("database.txt","a+");
    if(db == NULL) exit(-1);
    fprintf(db,"%s,%s,%d,%d;\n",username,password,porta,token);
    fclose(db);
    sprintf(sendline,"Ti sei registrato correttamente,%d\n",token);
    return;
}


void login(char sendline[],char *username,char* password,int porta){
    FILE *db = NULL;
    char line[500];
    char *token = NULL;
    int aux;
    long int codice;


    db = fopen("database.txt","r");
    if(db == NULL){
        return;
    }
    
    while (fgets(line,500,db)!=NULL)
    {  
        token = strtok(line,",");
        if(strcmp(username,token)==0){
            token = strtok(NULL,",");
            if(strcmp(password,token)==0){
                aux = atoi(strtok(NULL,","));
                if(aux == porta){
                    codice = atol(strtok(NULL,";"));
                    sprintf(sendline ,"Ti sei loggato correttamente,%ld;\n",codice);
                    fclose(db);
                    return;
                }
            }
        }
    }
    sprintf(sendline,"Errore non sei riuscito a loggarti\n");
    fclose(db);
    return;
}

void aggiungi_client(char *username,long int token,int porta,struct sockaddr_in6 remote_addr,Client *client){
    //troviamo il primo posto disponibile
    int i = 0;
    while(client[i].porta != -1){
        i++;
    }
    //abbiamo trovato l'indice per cui possiamo registrare il client
    strcpy(client[i].username,username);
    client[i].token = token;
    client[i].porta = porta;
    inet_ntop(AF_INET6,&(remote_addr.sin6_addr),client[i].ipv6_addr, INET6_ADDRSTRLEN);
    return;
}  



int main(int argc, char **argv){

    int sockfd,n;
    struct sockaddr_in6 local_addr, remote_addr;
    socklen_t len = sizeof(struct sockaddr_in6);
    char sendline[1000];
    char recvline[1000];
    char ipv6_addr[50];

    if(argc < 2){
        printf("Errore. Devi inserire il numero di porta\n");
        return -1;
    }

    sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sockfd < 0){
        printf("Errore nell'apertura di socket\n");
        return -1;
    }

    memset(&local_addr, 0, len);
    local_addr.sin6_family = AF_INET6;
    local_addr.sin6_port = htons(atoi(argv[1]));

    if(bind(sockfd, (struct sockaddr *) &local_addr, len) < 0){
        printf("Errore nell'apertura di bind()\n Utilizza un altro valore\n");
        return -1;
    }
    
    
    char *action;
    char *username;
    char *password;
    int porta;
    long int token;
    char messaggio[500];
    Client client[20];
    for (int i = 0; i < 20; i++)
    {
        client[i].porta = -1;
    }
    
    for(;;){
        if((n = recvfrom(sockfd, recvline, 999, 0, (struct sockaddr *) &remote_addr, &len))==0) exit(-1);
        recvline[n] = 0;
        inet_ntop(AF_INET6,&(remote_addr.sin6_addr),ipv6_addr, INET6_ADDRSTRLEN);
        printf("IP = %s Port = %d Msg = %s\n", ipv6_addr,ntohs(remote_addr.sin6_port), recvline);
        action = strtok(recvline,",");
        if(strcmp(action,"Reg") == 0){
            username = strtok(NULL,",");
            password = strtok(NULL,",");
            porta = atoi(strtok(NULL,";"));
            registrazione(username,password,porta,sendline);
            sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr *) &remote_addr, sizeof(remote_addr));
            if(strcmp("Ti sei registrato correttamente",strtok(sendline,","))==0){
                token = atol(strtok(NULL,";"));
                aggiungi_client(username,token,porta,remote_addr,client);
            }
        }
        else if(strcmp(action,"Log") == 0){
            username = strtok(NULL,",");
            password = strtok(NULL,",");
            porta = atoi(strtok(NULL,";"));
            login(sendline,username,password,porta);
            sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr *) &remote_addr, sizeof(remote_addr));
            if(strcmp("Ti sei loggato correttamente",strtok(sendline,","))==0){
                token = atol(strtok(NULL,";"));
                aggiungi_client(username,token,porta,remote_addr,client);
            }
            
        }
        else if(strcmp(action,"Msg")==0){
            token = atol(strtok(NULL,","));
            int i = 0;
            while (i<20 && token != client[i].token)
            {
                i++;
            }
            strcpy(username,client[i].username);
            strcpy(messaggio,strtok(NULL,";"));//salviamo il messaggio da inviare
            //cominciamo a scorrere l'array client
            inet_ntop(AF_INET6,&(remote_addr.sin6_addr),ipv6_addr,INET6_ADDRSTRLEN);//ci salviamo l'ip del mittente
            i = 0;

            while (i<20)
            {
                //Facciamo un controllo per evitare di mandarlo al mittente
                if(client[i].porta != -1 && strcmp(client[i].ipv6_addr,ipv6_addr)!=0){
                    inet_pton(AF_INET6,client[i].ipv6_addr,&remote_addr.sin6_addr);
                    remote_addr.sin6_port = htons(client[i].porta);
                    memset(sendline,0,999);
                    sprintf(sendline,"%s,%s\n",username,messaggio);
                    printf("BUCCHIN %s\n",sendline);
                    sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr *) &remote_addr, sizeof(remote_addr));

                }
                i++;
            }   
        }
        
    }

    close(sockfd);
    return 0;
}