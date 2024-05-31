#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>

int main(int argc, char **argv){

    int sockfd,n;
    struct sockaddr_in6 dest_addr;
    socklen_t len = sizeof(struct sockaddr_in6);
    char buffer[1000];

    if(argc < 3){
        printf("Devi Inserire IP e porta del server\n");
        return -1;
    }

    sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if(sockfd < 0){ // if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
        printf("Errore nell'apertura di socket!");
        return -1;
    }

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin6_family = AF_INET6;
    inet_pton(AF_INET6, argv[1], &(dest_addr.sin6_addr));
    dest_addr.sin6_port = htons(atoi(argv[2]));

    printf("Qual è il tuo username?\n");
    scanf("%s",buffer);
    strcat(buffer,";");
    sendto(sockfd,buffer,strlen(buffer)+1,0,(struct sockaddr *)&dest_addr,sizeof(dest_addr));
    n = recvfrom(sockfd,buffer,999,0,(struct sockaddr*)&dest_addr,&len);
    buffer[n] = 0;
    printf("%s\n",buffer);
    
    int riga;
    int colonna;

    for(;;){
        n = recvfrom(sockfd,buffer,999,0,(struct sockaddr*)&dest_addr,&len);
        buffer[n] = 0;
        printf("%s\n",buffer);
        if(strcmp(buffer,"Hai perso!\n") == 0 || strcmp(buffer,"Pareggio!\n")==0) break;
        printf("In che riga vuoi inserire il tuo segno?\n");
        scanf("%d",&riga);
        printf("In che colonna vuoi inserire il tuo segno?\n");
        scanf("%d",&colonna);

        sprintf(buffer,"%d,%d;",riga,colonna);
        sendto(sockfd,buffer,strlen(buffer)+1,0,(struct sockaddr *)&dest_addr,sizeof(dest_addr));
        n = recvfrom(sockfd,buffer,999,0,(struct sockaddr*)&dest_addr,&len);
        buffer[n] = 0;
        printf("%s\n",buffer);
        if(strcmp(buffer,"Hai vinto!\n") == 0 || strcmp(buffer,"Pareggio!\n")==0) break;

    }

    close(sockfd);


    return 0;
}