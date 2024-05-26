#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>





int main(int argc, char **argv){
    int status = 0;
    int sockfd;
    struct sockaddr_in dest_addr;
    char buffer[100];
    socklen_t len = sizeof(struct sockaddr_in);
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

    //sendto(sockfd, buffer, strlen(buffer)+1, 0, (struct sockaddr *)&dest_addr,sizeof(dest_addr));
    //recvfrom(sockfd, buffer, 99, 0, (struct sockaddr *) &dest_addr, &len);
    int n;
    char username[20];
    char password[20];
    for(;;){
        printf("Cosa vuoi fare?\n\n_____________\n\n1 Registrati\n\n2 effettua il login\n\n3 Leggi la bacheca\n\n4 Scrivi qualcosa\n\n_____________\n\n");
        scanf("%d",&n);
        switch(n){
            case 1:
                strcpy(buffer,"Registrazione");
                sendto(sockfd, buffer, strlen(buffer)+1, 0, (struct sockaddr *)&dest_addr,sizeof(dest_addr));
                printf("Inserisci il tuo username\n");
                scanf("%s",username);

                printf("Inserisci la password\n");
                scanf("%s",password);

                strcat(username,",");
                strcat(username,password);
                strcpy(buffer,username);

                sendto(sockfd, buffer, strlen(buffer)+1, 0, (struct sockaddr *)&dest_addr,sizeof(dest_addr));
                recvfrom(sockfd, buffer, 99, 0, (struct sockaddr *) &dest_addr, &len);
                printf("%s\n",buffer);
                break;
            case 2:
                strcpy(buffer,"Login");
                sendto(sockfd, buffer, strlen(buffer)+1, 0, (struct sockaddr *)&dest_addr,sizeof(dest_addr));
                printf("Inserisci il tuo username\n");
                scanf("%s",username);

                printf("Inserisci la password\n");
                scanf("%s",password);

                strcat(username,",");
                strcat(username,password);
                strcpy(buffer,username);

                sendto(sockfd, buffer, strlen(buffer)+1, 0, (struct sockaddr *)&dest_addr,sizeof(dest_addr));
                recvfrom(sockfd, buffer, 99, 0, (struct sockaddr *) &dest_addr, &len);
                printf("%s\n",buffer);
                if(strcmp(buffer,"Ti sei loggato correttamente")==0){
                    status = 1;
                }
                break;
            case 3:
                if(status == 0) break;
                strcpy(buffer,"Stampa messaggi");
                sendto(sockfd, buffer, strlen(buffer)+1, 0, (struct sockaddr *)&dest_addr,sizeof(dest_addr));
                int i = 0;
                while (strcmp(buffer,"FINE MESSAGGI\n")!=0)
                {
                    recvfrom(sockfd, buffer, 99, 0, (struct sockaddr *) &dest_addr, &len);
                    printf("%d: %s",i,buffer);
                    i++;
                }
                break;
            case 4:
                if(status == 0) break;
                strcpy(buffer,"Scrivi messaggio");
                sendto(sockfd, buffer, strlen(buffer)+1, 0, (struct sockaddr *)&dest_addr,sizeof(dest_addr));
                char msg[500];
                printf("Scrivi il messaggio che vuoi inviare\n");
                fgets(buffer,99,stdin);
                //scanf("%s",msg);
                fgets(msg,500,stdin);
                sendto(sockfd, msg, 500, 0, (struct sockaddr *)&dest_addr,sizeof(dest_addr));
                recvfrom(sockfd, buffer, 99, 0, (struct sockaddr *) &dest_addr, &len);
                printf("%s",buffer);
                break;
            default:
                printf("Hai inserito una scelta non valida!");
        }
    }
    close(sockfd);


    return 0;
}