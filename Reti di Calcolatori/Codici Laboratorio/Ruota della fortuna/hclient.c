#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<time.h>

int main(int argc, char **argv){

    int sockfd,n;
    srand(time(NULL));
    struct sockaddr_in dest_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    char username[50];
    char recvline[1000];
    char sendline[1000];

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
    
    printf("Inserisci il tuo username: ");
    scanf("%s",username);

    sendto(sockfd,username,strlen(username)+1,0,(struct sockaddr*)&dest_addr,sizeof(dest_addr));
    n = recvfrom(sockfd,recvline,999,0,(struct sockaddr*)&dest_addr,&len);
    recvline[n] = 0;
    printf("%s\n",recvline);
    
    char lettera;
    int montepremi;
    int ch;
    for(;;){
                
        n = recvfrom(sockfd,recvline,999,0,(struct sockaddr*)&dest_addr,&len);
        recvline[n] = 0;
        printf("%s\n",recvline);
        if(strcmp(strtok(recvline,"!"),"HAI VINTO")==0 || strcmp(strtok(recvline,"!"),"HAI PERSO")==0)exit(-1);
        int ch;
        while((ch = getchar())!=EOF && ch != '\n');
        printf("Inserisci la lettera che vuoi inviare\n");
        scanf("%c",&lettera);
        fflush(stdin);
        montepremi = rand()%(10000 - 1000 + 1)+1000;
        printf("Il tuo montepremi per questa lettera parte da %d\n",montepremi);
        memset(sendline,0,999);
        sprintf(sendline,"%c,%d;",lettera,montepremi);
        sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr*)&dest_addr,sizeof(dest_addr));
        n = recvfrom(sockfd,recvline,999,0,(struct sockaddr*)&dest_addr,&len);
        recvline[n] = 0;
        printf("%s\n",recvline);
    }

    close(sockfd);


    return 0;
}