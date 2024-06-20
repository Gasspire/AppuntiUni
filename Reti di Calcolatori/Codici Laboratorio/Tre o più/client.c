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

    char username[50];
    printf("Inserisci il tuo username: ");
    scanf("%s",username);
    sendto(sockfd,username,strlen(username)+1,0,(struct sockaddr*)&dest_addr,sizeof(dest_addr));
    n = recvfrom(sockfd,buffer,999,0,(struct sockaddr*)&dest_addr,&len);
    buffer[n]=0;
    printf("%s\n",buffer);

    int si,sj;
    int di,dj;

    for(;;){
        n = recvfrom(sockfd,buffer,999,0,(struct sockaddr*)&dest_addr,&len);
        buffer[n]=0;
        printf("%s\n",buffer);
        if(strcmp(buffer,"HAI VINTO!")==0 || strcmp(buffer,"HAI PERSO!")==0 || strcmp(buffer,"PAREGGIO!")==0) return 0;
        printf("Inserisci le coordinate del gettone che vuoi spostare\n");
        printf("i: ");
        scanf("%d",&si);
        printf("j: ");
        scanf("%d",&sj);
        printf("Inserisci le coordinate dove vuoi posizionare il tuo gettone (deve essere vuoto)\n");
        printf("i: ");
        scanf("%d",&di);
        printf("j: ");
        scanf("%d",&dj);
        memset(buffer,0,999);
        sprintf(buffer,"%d,%d,%d,%d;",si,sj,di,dj);
        sendto(sockfd,buffer,strlen(buffer)+1,0,(struct sockaddr*)&dest_addr,sizeof(dest_addr));
        n = recvfrom(sockfd,buffer,999,0,(struct sockaddr*)&dest_addr,&len);
        buffer[n]=0;
        printf("%s\n",buffer);
    }

    close(sockfd);


    return 0;
}