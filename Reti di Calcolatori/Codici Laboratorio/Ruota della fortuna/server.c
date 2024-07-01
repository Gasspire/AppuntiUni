#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <time.h>

typedef struct giocatore
{
    char username[50];
    int montepremi;
    char ip_addr[50];
    int porta;
} Giocatore;


void PrintStato(char sendline[],char fraseattuale[],Giocatore player){
    memset(sendline,0,999);
    sprintf(sendline,"%s\n%s il tuo monte premi è: %d",fraseattuale,player.username,player.montepremi);
}

int main(int argc, char **argv){
    srand(time(NULL));
    int sockfd,n;
    struct sockaddr_in local_addr, remote_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    char sendline[1000];
    char recvline[1000];
    if (argc < 2)
    {
        printf("Errore. Devi inserire il numero di porta\n");
        return -1;
    }

    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        printf("Errore nell'apertura di socket\n");
        return -1;
    }

    memset(&local_addr, 0, len);
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(atoi(argv[1]));

    if (bind(sockfd, (struct sockaddr *)&local_addr, len) < 0)
    {
        printf("Errore nell'apertura di bind()\n Utilizza un altro valore\n");
        return -1;
    }

    /*for(;;){
        recvfrom(sockfd, buffer, 99, 0, (struct sockaddr *) &remote_addr, &len);
        printf("IP = %s Port = %d Msg = %s\n", inet_ntoa(remote_addr.sin_addr),
            ntohs(remote_addr.sin_port), buffer);
    }*/

    int numerogiocatori = 0;
    Giocatore giocatori[2];
    while (numerogiocatori < 2)
    {
        n = recvfrom(sockfd, recvline, 999, 0, (struct sockaddr *) &remote_addr, &len);
        recvline[n] = 0;
        printf("Ho ricevuto %s da %s\n",recvline,inet_ntoa(remote_addr.sin_addr));
        strcpy(giocatori[numerogiocatori].username,recvline);
        strcpy(giocatori[numerogiocatori].ip_addr,inet_ntoa(remote_addr.sin_addr));
        giocatori[numerogiocatori].porta = ntohs(remote_addr.sin_port);
        giocatori[numerogiocatori].montepremi = 0;
        sprintf(sendline,"Ti sei registrato correttamente %s",giocatori[numerogiocatori].username);
        sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr *) &remote_addr,sizeof(remote_addr));
        numerogiocatori++;

    }
    //fine registrazione

    char FrasiDisponibili[5][50] = {"superero questo esame","un lannister paga sempre i suoi debiti","l'inverno sta arrivando","senti il mio ruggito","reti di calcolatori"};

    int numerofrase = rand()%5;
    char frasemisteriosa[strlen(FrasiDisponibili[numerofrase])+1];
    strcpy(frasemisteriosa,FrasiDisponibili[numerofrase]);
    printf("la frase è %s\n",frasemisteriosa);

    char fraseattuale[strlen(frasemisteriosa)+1];
    for (int i = 0; i < strlen(frasemisteriosa); i++)
    {
        if(((int) frasemisteriosa[i]) >= ((int) 'a') && ((int) frasemisteriosa[i]) <= ((int)'z')){
            fraseattuale[i] = '_';
        }
        else{
            fraseattuale[i] = ' ';
        }
    }
    fraseattuale[strlen(frasemisteriosa)] = '\0';
    printf("lo stato è %s\n",fraseattuale);
    


    int turno = 0;
    char lettera;
    char *aux;
    int monteaux;
    int contatore = 0;
    int finegioco =0;
    for (;;)
    {  
        //settiamo la socket 
        inet_pton(AF_INET, giocatori[turno].ip_addr, &(remote_addr.sin_addr));
        remote_addr.sin_port = htons(giocatori[turno].porta);
        PrintStato(sendline,fraseattuale,giocatori[turno]);
        sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr *) &remote_addr,sizeof(remote_addr));
        n = recvfrom(sockfd, recvline, 999, 0, (struct sockaddr *) &remote_addr, &len);
        //facciamo il controllo
        recvline[n] = 0;
        aux = strtok(recvline,",");
        lettera = aux[0];
        monteaux = atoi(strtok(NULL,";"));

        for(int i = 0; i<strlen(fraseattuale); i++){
            if(frasemisteriosa[i] == lettera && fraseattuale[i] == '_'){
                contatore++;
                fraseattuale[i] = lettera;
            }
        }
        monteaux = monteaux*contatore;
        if(contatore == 0){
            memset(sendline,0,999);
            sprintf(sendline,"Non hai indovinato nessuna lettera o la lettera è gia stata indovinata, il turno passerà al prossimo giocatore");
            sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr *) &remote_addr,sizeof(remote_addr));
            turno = (turno+1)%2;
        }
        else{
            memset(sendline,0,999);
            giocatori[turno].montepremi+=monteaux;
            sprintf(sendline,"Hai indovinato %d lettera/e, il tuo montepremi è di %d",contatore,giocatori[turno].montepremi);
            sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr *) &remote_addr,sizeof(remote_addr));
            contatore = 0;
            memset(sendline,0,999);
            finegioco = 0;
            for(int i = 0; i<strlen(fraseattuale);i++){
                if(fraseattuale[i] == '_') finegioco  = 1;
            }
            if(finegioco==0){
                sprintf(sendline,"HAI VINTO!\nIl tuo montepremi è di : %d",giocatori[turno].montepremi);
                sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr *) &remote_addr,sizeof(remote_addr));
                inet_pton(AF_INET, giocatori[(turno+1)%2].ip_addr, &(remote_addr.sin_addr));
                remote_addr.sin_port = htons(giocatori[(turno+1)%2].porta);
                memset(sendline,0,999);
                sprintf(sendline,"HAI PERSO!\nLa vittoria è di : %s",giocatori[turno].username);
                sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr *) &remote_addr,sizeof(remote_addr));
                exit(-1);
            }
        }

    }

    close(sockfd);
    return 0;
}