#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>

int main(int argc, char **argv){

    int sockfd,n;
    struct sockaddr_in dest_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    char buffer[500];
    char messaggio[400];
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
    
    /*sprintf(buffer, "%d", i);
    printf("Stiamo inviando al server = ", buffer);
    sendto(sockfd, buffer, strlen(buffer)+1, 0, (struct sockaddr *)&dest_addr,sizeof(dest_addr));*/
    int scelta;
    for(;;){
        printf("Cosa vuoi fare?\n\n1 Invio messaggio\n2 Stampa lista messaggi\n3 Modifica messaggi\n4 Cancella messaggio\n5 Chiudi connessione\n");
        scanf("%d",&scelta);
        int c;
        int numero_messaggio;
        switch (scelta)
        {
        case 1:
            printf("Che messaggio vuoi inviare?\n");
            while((c = getchar())!= '\n' && c != EOF);
            fgets(messaggio,400,stdin);
            messaggio[strlen(messaggio)-1] = ';';
            sprintf(buffer,"Invio,%s",messaggio);
            sendto(sockfd, buffer, strlen(buffer)+1, 0, (struct sockaddr *)&dest_addr,sizeof(dest_addr));
            n =  recvfrom(sockfd,buffer,399,0,(struct sockaddr *)&dest_addr,&len);
            buffer[n] = 0;
            printf("%s\n",buffer);
            break;
        
        case 2:
            sprintf(buffer,"Stampa");
            sendto(sockfd, buffer, strlen(buffer)+1, 0, (struct sockaddr *)&dest_addr,sizeof(dest_addr));
            n =  recvfrom(sockfd,buffer,399,0,(struct sockaddr *)&dest_addr,&len);
            buffer[n] = 0;
            printf("%s\n",buffer);
            break;
        case 3:
            printf("Quale messaggio vuoi modificare?\n");
            scanf("%d",&numero_messaggio);
            printf("Cosa vuoi scrivere al suo posto?\n");
            while((c = getchar())!= '\n' && c != EOF);
            fgets(messaggio,400,stdin);
            messaggio[strlen(messaggio)-1] = ';';
            sprintf(buffer,"Modifica,%d,%s",numero_messaggio,messaggio);
            sendto(sockfd, buffer, strlen(buffer)+1, 0, (struct sockaddr *)&dest_addr,sizeof(dest_addr));
            n =  recvfrom(sockfd,buffer,399,0,(struct sockaddr *)&dest_addr,&len);
            buffer[n] = 0;
            printf("%s\n",buffer);
            break;
        case 4:
            printf("Quale messaggio vuoi eliminare?\n");
            scanf("%d",&numero_messaggio);
            sprintf(buffer,"Cancella,%d,%s",numero_messaggio,messaggio);
            sendto(sockfd, buffer, strlen(buffer)+1, 0, (struct sockaddr *)&dest_addr,sizeof(dest_addr));
            n =  recvfrom(sockfd,buffer,399,0,(struct sockaddr *)&dest_addr,&len);
            buffer[n] = 0;
            printf("%s\n",buffer);
            break;
        case 5:
            close(sockfd);
            return 0;
            break;
        default:
            printf("Devi fare una scelta valida\n");
            break;
        }
    }
    return 0;
}