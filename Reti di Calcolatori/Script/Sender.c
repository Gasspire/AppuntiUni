#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>


struct persona{
    char nome[10];
    char cognome[10];
    int eta;
};



int main(int argc,char **argv){
    int sockfd; //contiene numero di porta
    struct sockaddr_in dest_addr; //conterrà la famiglia e tutte le altre info ip e porta
    char buffer[100];//buffer di memoria
    //struct persona p;
    if(argc < 3){
        printf("Errore! Inserisci IP e porta \n");
        return -1;
    }

    sockfd = socket(PF_INET, SOCK_DGRAM, 0);    //socket prende famiglia e protocollo. AF_INET E PF_INET SONO UGUALI

    if(sockfd<0){
        printf("Errore nell'apertura di socket\n");
        return -1;
    }
    
    //dobbiamo andare a settare indirizzo ip e porta adesso
    
    memset(&dest_addr,0,sizeof(dest_addr)); //impostiamo tutto a 0 perché nella struct altrimenti rischiamo dati unexpected 
    dest_addr.sin_family = AF_INET;
    inet_pton(AF_INET,argv[1], &(dest_addr.sin_addr));//prendiamo l'indirizzo ip e lo trasformiamo da presentation a network. come primo parametro prendiamo il tipo di ip
    dest_addr.sin_port = htons(atoi(argv[2])); //converte la porta in numero comprensibile a tutte le macchine. Facciamo la conversione perché gli argv sono stringhe

    //mandiamo adesso dei pacchetti
    for (int i = 0; i < 10; i++)
    {  
        sprintf(buffer,"%d",i); // riempiamo il buffer delle cose che dobbiamo mandare
        sendto(sockfd,buffer, sizeof(buffer)+1,0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    }
    close(sockfd);


    return 0;
}