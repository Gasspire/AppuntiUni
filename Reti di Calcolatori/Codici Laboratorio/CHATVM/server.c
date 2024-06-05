#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<time.h>


typedef struct macchina{
    char nome[50];
    char ip[50];
    int porta;
    int distanze[6];
}Macchina;







int main(int argc, char **argv){

    int sockfd,n;
    struct sockaddr_in local_addr, remote_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    char buffer[1000];
    srand(time(NULL));
    if(argc < 2){
        printf("Errore. Devi inserire il numero di porta\n");
        return -1;
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
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
    int connessi = 0;
    Macchina macchine[6];
    strcpy(macchine[0].nome,"C");
    strcpy(macchine[1].nome,"C++");
    strcpy(macchine[2].nome,"Java");
    strcpy(macchine[3].nome,"Python");
    strcpy(macchine[4].nome,"R");
    strcpy(macchine[5].nome,"Matlab");
    for (int i = 0; i < 6; i++)
    {
        macchine[i].porta = -1;
    }
    
    for(;;){
        if( (n = recvfrom(sockfd, buffer, 999, 0, (struct sockaddr *) &remote_addr, &len)) <0) printf("Errore nella ricezione\n");
        buffer[n] = 0;
        printf("Ho ricevuto %s\n",buffer);
        char action[20];
        strcpy(action,strtok(buffer,","));
        if(strcmp("Registrazione",action)== 0){
            char *token = strtok(NULL,",");
            int i = 0;
            while(i < 6 && strcmp(token,macchine[i].nome)!=0){
                i++;
            }
            if(macchine[i].porta != -1){
                strcpy(buffer,"Errore, non hai scelto un linguaggio valido\n");
                sendto(sockfd,buffer,strlen(buffer)+1,0,(struct sockaddr*)&remote_addr,sizeof(remote_addr));
                continue;
            }
            if(i<6){ //l'utente ha fatto una scelta tra quelle offerte
                strcpy(macchine[i].ip,inet_ntoa(remote_addr.sin_addr));   //vengono impostati ip e porta
                //macchine[i].porta = ntohs(remote_addr.sin_port);
                macchine[i].porta = atoi(strtok(NULL,";"));
                //impostiamo le distanze di tutte le macchine che ci sono
                //in realtà potremmo assegnare a tutti già una distanza da ora per facilità. Ma non lo faremo
                for(int j = 0; j<6; j++){
                    if(macchine[j].porta > 0){
                        macchine[i].distanze[j] = (rand()%255)+1;
                    }
                    else macchine[i].distanze[j] = -1;
                }
                
                //adesso aggiorniamo le altre macchine per quanto riguarda la  distanza del nuovo arrivo
                for(int j = 0; j<6;j++){
                    if(macchine[j].porta > 0){
                        macchine[j].distanze[i] = macchine[i].distanze[j];
                    }                    
                }
                macchine[i].distanze[i] = -1;
                connessi++;
                strcpy(buffer,"Ti sei registrato correttamente!\n");
                sendto(sockfd,buffer,strlen(buffer)+1,0,(struct sockaddr*)&remote_addr,sizeof(remote_addr));
            }
            else{
                //l'utente non ha fatto una scelta valida
                strcpy(buffer,"Errore, non hai scelto un linguaggio valido\n");
                sendto(sockfd,buffer,strlen(buffer)+1,0,(struct sockaddr*)&remote_addr,sizeof(remote_addr));
            }
        }
        else if(strcmp("Messaggio",action)==0){
            int numero_macchine = atoi(strtok(NULL,","));
            if(numero_macchine > connessi || numero_macchine < 1){
                strcpy(buffer,"Numero di macchine non valido!\n");
                sendto(sockfd,buffer,strlen(buffer)+1,0,(struct sockaddr*)&remote_addr,sizeof(remote_addr));
                continue;
            }
            char *messaggio = strtok(NULL,";");
            if (messaggio == NULL) {
                strcpy(buffer, "Messaggio non specificato!\n");
                sendto(sockfd, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)&remote_addr, len);
                continue;
            }

            //Troviamo la macchina che ha inviato il messaggio
            int index = 0;
            while (index < 6 && strcmp(macchine[index].ip,inet_ntoa(remote_addr.sin_addr))!=0)
            {
                index++;
            }
            if(index >= 6){ 
                printf("Errore, accesso illecito\n");
                continue;
            }

            int visitato[6] = {0};
            visitato[index] = 1; //settiamo la macchina stessa come se fosse già stata visitata, in modo da evitare che venga presa
            printf("Il messaggio che sto per mandare è: %s\n",messaggio);
            for(int i = 0; i < numero_macchine; i++){
                int minimo = 3000;
                int minindex = -1;
                for(int j = 0; j < 6; j++){
                    //se è effettivamente minore di quello attuale e non è stato visitato sostituiscilo
                    if(macchine[index].distanze[j] > 0 && macchine[index].distanze[j]<minimo && visitato[j] != 1){
                        minimo = macchine[index].distanze[j];
                        minindex = j;
                    } 
                }
                visitato[minindex] = 1;
                inet_pton(AF_INET,macchine[minindex].ip,&remote_addr.sin_addr);
                remote_addr.sin_port = htons(macchine[minindex].porta);
                if(sendto(sockfd,messaggio,strlen(messaggio)+1,0,(struct sockaddr*)&remote_addr,sizeof(remote_addr)) < 0){
                    printf("Errore nella send\n");
                }
            }
        }
        else{
            strcpy(buffer,"Errore, azione illecita\n");
            sendto(sockfd,buffer,strlen(buffer)+1,0,(struct sockaddr*)&remote_addr,sizeof(remote_addr));
        }
    }

    close(sockfd);
    return 0;
}