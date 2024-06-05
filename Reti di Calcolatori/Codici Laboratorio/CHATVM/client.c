#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>

int main(int argc, char **argv){

    int sockfd, n;
    struct sockaddr_in local_addr, remote_addr;
    socklen_t len =  sizeof(struct sockaddr_in);
    char msg[1000];

    if(argc < 4){ //ipserver,portaserver,portaricezione
        printf("Errore! Inserisci tutti i parametri\n");
        return -1;
    }
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        printf("Errore nell'apertura di socket\n");
        return -1;
    }

    if(fork()){
        memset(&local_addr, 0, len);
        local_addr.sin_family = AF_INET;
        local_addr.sin_port = htons(atoi(argv[3])); 

        if(bind(sockfd, (struct sockaddr *)&local_addr, len) < 0){
            printf("Errore nella bind\n");
            return -1;
        }

        n = recvfrom(sockfd, msg, 999, 0,(struct sockaddr *)&remote_addr, &len);
        msg[n] = 0;
        printf("%s\n",msg);
        if(strcmp(msg,"Errore, non hai scelto un linguaggio valido\n")==0)exit(-1);
        for(;;){
            n = recvfrom(sockfd, msg, 999, 0,(struct sockaddr *)&remote_addr, &len);
            msg[n] = 0;
            printf("MSG = %s\n", msg);

        }
        close(sockfd);
        return 0;
    }
    else{
        sleep(1);

        memset(&remote_addr, 0, len);
        remote_addr.sin_family = AF_INET;
        inet_pton(AF_INET, argv[1], &(remote_addr.sin_addr));
        remote_addr.sin_port = htons(atoi(argv[2]));

        char linguaggio[50];
        printf("Con quale linguaggio vuoi loggare?\n");
        scanf("%s",linguaggio);
        sprintf(msg,"Registrazione,%s,%d;\n",linguaggio,atoi(argv[3]));
        sendto(sockfd, msg, strlen(msg), 0,(struct sockaddr *)&remote_addr,len);
        int num_macchine;
        char messaggio[500];

        for(;;){
            printf("Quante macchine vuoi contattare?\n");
            scanf("%d",&num_macchine);
            printf("Inserisci il messaggio che vuoi inviare: ");
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            fgets(messaggio,499,stdin);
            messaggio[strlen(messaggio)-1] = ';'; //togliamo il \n dalla stringa del messaggio
            sprintf(msg,"Messaggio,%d,%s", num_macchine,messaggio);
            sendto(sockfd, msg, strlen(msg), 0,(struct sockaddr *)&remote_addr,len);

        }
        printf("Fine della ricezione\n");
        close(sockfd);
        return 0;
    }
}