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

    int stato = 0;
    int sockfd;
    struct sockaddr_in dest_addr;
    char buffer[100];

    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    ControlloAperturaSocket(sockfd);

    memset(&dest_addr,0,sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    inet_pton(AF_INET,argv[1], &(dest_addr.sin_addr));
    dest_addr.sin_port = htons(atoi(argv[2]));

    int comando;
    
    for(;;){   
        printf("Inserisci il comando da eseguire \n 1 per registrare \n2 per mandare la mossa\n");
        scanf("%d",&comando);

        switch (comando)
        {
        case 1:
            if(stato == 1) break;
            char *roba = "R"
            sendto(sockfd,roba,sizeof(roba)+1,0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
            stato = 1;
            break;
        
        case 2:
            printf("Inserisci mossa da fare \n");
            scanf("%s",buffer);
            sendto(sockfd,buffer,sizeof(buffer)+1,0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
            break;
        }


    }
    close(sockfd);
}