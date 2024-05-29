#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<arpa/inet.h>


typedef struct client{
    char nome[50];
    char ip[50];
    int porta;
    int vite;
}Client;



void inviostato(char sendline[],char parola[],Client Giocatore){
    memset(sendline,0,1000);
    sprintf(sendline,"Lo stato attuale della parola è %s\nTi sono rimaste %d vite!\n",parola,Giocatore.vite);
}

int verifica(char recvline[],char parola_magica[],char parola[],Client *Giocatore, char sendline[]){
    int lunghezza = strlen(parola_magica);
    int presente = 0; 
    if(strlen(recvline)> 1) {
        if(strcmp(recvline,parola_magica)==0){
            strcpy(sendline,"HAI VINTO!\n");
            return 1;
        }
        else{
            strcpy(sendline,"Parola errata!\n");
            Giocatore->vite--;
        }
        return 0;
    }
    else{
        char lettera = recvline[0];
        for(int i = 0; i< lunghezza; i++){
            if(parola_magica[i] == lettera){
                presente = 1;
                parola[i] = lettera;
            }
        }
        if(presente == 0){
            Giocatore->vite--;
            strcpy(sendline,"La lettera inserita non è presente nella parola!\n");
            return 0;
        }
        else{
            memset(sendline,0,1000);
            sprintf(sendline,"La lettera inserita è presente %s\n",parola);
            return 0;
        }
    }
}


int main(int argc, char **argv){
    srand(time(NULL));
    int sockfd,n;
    struct sockaddr_in local_addr, remote_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    char sendline[1000];
    char recvline[1000];

    if(argc < 2){
        printf("Errore. Devi inserire il numero di porta\n");
        return -1;
    }

    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0){
        printf("Errore nell'apertura di socket\n");
        return -1;
    }

    memset(&local_addr, 0, len);
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(atoi(argv[1]));

    if(bind(sockfd, (struct sockaddr *) &local_addr, len) < 0){
        printf("Errore nell'apertura di bind()\n Utilizza un altro valore\n");
        return -1;
    }


    //INIZIO REGISTRAZIONE 
    Client giocatori[2];
    int numerogiocatori = 0;
    while (numerogiocatori < 2)
    {
        n = recvfrom(sockfd,recvline,999,0,(struct sockaddr *)&remote_addr,&len);
        recvline[n] = 0;
        strcpy(giocatori[numerogiocatori].nome,strtok(recvline,";"));
        strcpy(giocatori[numerogiocatori].ip,inet_ntoa(remote_addr.sin_addr));
        giocatori[numerogiocatori].porta = ntohs(remote_addr.sin_port);
        giocatori[numerogiocatori].vite = 3;
        memset(sendline,0,sizeof(sendline));
        sprintf(sendline,"Ti sei registrato correttamente come : %s",giocatori[numerogiocatori].nome);
        sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr *)&remote_addr,sizeof(remote_addr));
        numerogiocatori++;
    }
    //inizializzazione del gioco
    char parole_da_indovinare[5][20] = {"prosciutto","ciao","calcolatore","manzoni","scienziato"};
    char parola_magica[20];
    strcpy(parola_magica,parole_da_indovinare[rand()%5]);





    //recvfrom(sockfd, buffer, 99, 0, (struct sockaddr *) &remote_addr, &len);
    //printf("IP = %s Port = %d Msg = %s\n", inet_ntoa(remote_addr.sin_addr),ntohs(remote_addr.sin_port), buffer);
    int turno = 0;
    int lunghezza_parola = strlen(parola_magica);
    char parola[lunghezza_parola+1];
    for(int i = 0; i<lunghezza_parola;i++) parola[i] = '_';
    parola[lunghezza_parola] = '\0';

    for(;;){
        inet_pton(AF_INET,giocatori[turno].ip,&remote_addr.sin_addr);
        remote_addr.sin_port = htons(giocatori[turno].porta);

        inviostato(sendline,parola,giocatori[turno]);
        sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr *) &remote_addr,sizeof(remote_addr));

        n = recvfrom(sockfd, recvline, 999, 0, (struct sockaddr *) &remote_addr, &len);
        recvline[n] = 0;
        if(verifica(recvline,parola_magica,parola,&giocatori[turno],sendline)==1){
            strcpy(sendline,"HAI VINTO\n");
            sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr *) &remote_addr,sizeof(remote_addr));
            inet_pton(AF_INET,giocatori[(turno+1)%2].ip,&remote_addr.sin_addr);
            remote_addr.sin_port = htons(giocatori[(turno+1)%2].porta);
            strcpy(sendline,"HAI PERSO\n");
            sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr *) &remote_addr,sizeof(remote_addr));
            return 0;

        }
        else{
            if(giocatori[turno].vite == 0) strcpy(sendline,"HAI PERSO\n");
            sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr *) &remote_addr,sizeof(remote_addr));  
        }

        if(giocatori[(turno+1)%2].vite > 0) turno = (turno+1)%2;
        if(giocatori[(turno+1)%2].vite == 0 && giocatori[turno].vite == 0){
            printf("ENTRAMBI I GIOCATORI HANNO PERSO\n");
            return 0;
        }
    }

    close(sockfd);
    return 0;
}