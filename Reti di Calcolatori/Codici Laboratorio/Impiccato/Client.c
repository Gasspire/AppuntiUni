#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>

int main(int argc, char **argv){

    int sockfd,n;
    struct sockaddr_in dest_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    char buffer[1000];

    if(argc < 3){
        printf("Devi Inserire IP e porta del server\n");
        return -1;
    }

    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){ // if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
        printf("Errore nell'apertura di socket!");
        return -1;
    }

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &(dest_addr.sin_addr));
    dest_addr.sin_port = htons(atoi(argv[2]));


    printf("Qual è il tuo username?\n");
    scanf("%s",buffer);
    strcat(buffer,";");
    sendto(sockfd,buffer,strlen(buffer)+1,0,(struct sockaddr*)&dest_addr,sizeof(dest_addr));
    n = recvfrom(sockfd,buffer,1000,0,(struct sockaddr*)&dest_addr,&len);
    buffer[n] = 0;
    printf("%s\n",buffer);

    for(;;){
        n = recvfrom(sockfd,buffer,999,0,(struct sockaddr*)&dest_addr,&len);
        buffer[n] = 0;
        printf("%s\n",buffer);
        if(strcmp(buffer,"HAI PERSO\n") == 0) return 0;
        printf("Inserisci il carattere o la parola che vuoi inviare\n");
        scanf("%s",buffer);
        sendto(sockfd,buffer,strlen(buffer)+1,0,(struct sockaddr*)&dest_addr,sizeof(dest_addr));
        n = recvfrom(sockfd,buffer,999,0,(struct sockaddr*)&dest_addr,&len);
        buffer[n] = 0;
        printf("%s\n",buffer);
        if(strcmp(buffer,"HAI VINTO\n") == 0 || strcmp(buffer,"HAI PERSO\n") == 0) return 0;
    }

    close(sockfd);


    return 0;
}