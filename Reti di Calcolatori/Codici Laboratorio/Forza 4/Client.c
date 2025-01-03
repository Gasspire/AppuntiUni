#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

void ControlloInserimentoIPPorta(int argc){
    if(argc < 3 ){
        printf("errore, inserisci l'ip e la porta da riga da comando\n");
        exit(-1);
    }
}
void ControlloAperturaSocket(int sockfd){
    if(sockfd < 0){
        printf("Errore nell'apertura di socket \n");
        exit(-1);
    }
}


int main(int argc,char **argv){

    ControlloInserimentoIPPorta(argc);
    int sockfd,n;
    struct sockaddr_in dest_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    char buffer[1000];

    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    ControlloAperturaSocket(sockfd);

    memset(&dest_addr,0,sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    inet_pton(AF_INET,argv[1], &(dest_addr.sin_addr));
    dest_addr.sin_port = htons(atoi(argv[2]));
    

    printf("Qual è il tuo username?\n");
    scanf("%s",buffer);
    strcat(buffer,";");
    sendto(sockfd,buffer,strlen(buffer)+1,0,(struct sockaddr*)&dest_addr,sizeof(dest_addr));
    n = recvfrom(sockfd,buffer,1000,0,(struct sockaddr*)&dest_addr,&len);
    buffer[n] = 0;
    printf("%s\n",buffer);

    for(;;){   
        n = recvfrom(sockfd,buffer,1000,0,(struct sockaddr*)&dest_addr,&len);
        buffer[n] = 0;
        printf("%s\n",buffer);
        
        printf("Qual è la tua mossa?\n");
        scanf("%s",buffer);
        sendto(sockfd,buffer,strlen(buffer)+1,0,(struct sockaddr*)&dest_addr,sizeof(dest_addr));

        n = recvfrom(sockfd,buffer,999,0,(struct sockaddr*)&dest_addr,&len);
        buffer[n] = 0;
        printf("%s\n",buffer);
    }
    close(sockfd);
}