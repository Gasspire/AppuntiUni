#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

/*struct persona{
    char nome[10];
    char cognome[10];
    int eta;
};*/

int main(int argc,char **argv){
    int sockfd;
    struct sockaddr_in local_addr, remote_addr; //queste variabili contengono le informazioni in uno del server stess e in uno del client
    socklen_t len = sizeof(struct sockaddr_in);
    char buffer[100];
    //struct persona p;

    if(argc <2){ //sta volta abbiamo bisogno solo del numero di porta e non dell'indirizzo
        printf("Errore, inserisci il numero di porta\n");
        return -1;
    }

    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd < 0){
        printf("Errore nell'apertura di socket \n");
        return -1;
    }



    memset(&local_addr,0,len);

    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(atoi(argv[1]));

    if(bind(sockfd, (struct sockaddr*)&local_addr, len)<0){
        printf("Errore Inserisci un numero di porta differente\n");
        return -1;
    }

    for(;;){    
        recvfrom(sockfd, buffer, 99, 0, (struct sockaddr*)&remote_addr,&len);
        //recvfrom(sockfd,&p,sizeof(p)+1,0,(struct sockaddr*)&remote_addr,&len);
        printf("IP = %s port = %d nome = %s cognome = %s eta = %d \n", inet_ntoa(remote_addr.sin_addr), ntohs(remote_addr.sin_port),p.nome,p.cognome,p.eta);
    }

    close(sockfd);
    return 0;
}