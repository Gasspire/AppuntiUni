#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>



typedef struct Player{
    char username[20];
    char ip[INET6_ADDRSTRLEN];
    int port;
    char segno;
}Player;


char **InizioCampoDaGioco(){
    char **CampoDaGioco =(char **) malloc(sizeof(char *)*3);
    for(int i = 0; i<3; i++){
        CampoDaGioco[i] =(char *) malloc(sizeof(char)*3);
    }

    for(int i = 0; i<3;i++){
        for(int j = 0; j<3;j++){
            CampoDaGioco[i][j] = '-';
        }
    }
    return CampoDaGioco;
}



void PrintCampoDaGioco(char *sendline,char **CampoDaGioco){
    char aux[500];
    memset(sendline,0,1000);
    sprintf(aux,"    0 | 1 | 2 \n");
    strcat(sendline,aux);

    for(int i = 0; i<3; i++){
        sprintf(aux,"%d   %c | %c | %c\n",i, CampoDaGioco[i][0],CampoDaGioco[i][1],CampoDaGioco[i][2]);
        strcat(sendline,aux);
        if(i != 2){
            sprintf(aux,"   ------------\n"); 
            strcat(sendline,aux);
        }
    }
}

int ControllaMossa(int riga,int colonna,char **CampoDaGioco){
    if(riga < 0 || riga > 2 || colonna < 0 || colonna > 2) return 0;
    if(CampoDaGioco[riga][colonna] != '-') return 0;
    return 1;
}

int controlloorizzontale(int riga,int colonna,char segno,char **CampoDaGioco){
    while(riga > 0 && CampoDaGioco[riga][colonna] == segno){
        riga--;
    }
    int contatore = 0;
    while(riga < 3 && CampoDaGioco[riga][colonna] == segno){
        contatore++;
        riga++;
    }
    if (contatore == 3) return 1;
    return 0;
}
int controlloverticale(int riga,int colonna,char segno,char **CampoDaGioco){
    while(colonna > 0 && CampoDaGioco[riga][colonna] == segno){
        colonna--;
    }
    int contatore = 0;
    while(colonna < 3 && CampoDaGioco[riga][colonna] == segno){
        contatore++;
        colonna++;
    }
    if (contatore == 3) return 1;
    return 0;
}
int controllodiagdx(int riga,int colonna,char segno,char **CampoDaGioco){
    while(colonna < 2 && riga < 2 && CampoDaGioco[riga][colonna] == segno){
        colonna++;
        riga++;
    }
    int contatore = 0;
    while(colonna >= 0 && riga >=0 && CampoDaGioco[riga][colonna] == segno){
        colonna--;
        riga--;
        contatore++;
    }
    if (contatore == 3) return 1;
    return 0;
}
int controllodiagsx(int riga,int colonna,char segno,char **CampoDaGioco){
    while(colonna > 0 && riga < 2 && CampoDaGioco[riga][colonna] == segno){
        colonna--;
        riga++;
    }
    int contatore = 0;
    while(colonna >= 0 && riga >=0 && CampoDaGioco[riga][colonna] == segno){
        colonna++;
        riga--;
        contatore++;
    }
    if (contatore == 3) return 1;
    return 0;
}


int VerificaVittoria(int riga,int colonna,char segno, char **CampoDaGioco){
    if(controlloorizzontale(riga,colonna,segno,CampoDaGioco) == 1 || controlloverticale(riga,colonna,segno,CampoDaGioco) == 1) return 1;
    if(controllodiagsx(riga,colonna,segno,CampoDaGioco) == 1 || controllodiagdx(riga,colonna,segno,CampoDaGioco) == 1) return 1;
    return 0;
}

int giocofinito(char **CampoDaGioco){
    int controllo = 0;
    for(int i = 0; i<3;i++){
        for(int j = 0; j<3;j++){
            if(CampoDaGioco[i][j] == '-') controllo = 1;
        }
    }
    if(controllo == 0) return 1;
    else return 0;
}
int Mossa(int riga,int colonna, char **CampoDaGioco,Player Giocatore,char *sendline){
    strcpy(sendline,"La tua mossa non è andata a buon fine!");
    if(ControllaMossa(riga,colonna,CampoDaGioco)==0) return -1;
    else{
        CampoDaGioco[riga][colonna] = Giocatore.segno;
        strcpy(sendline,"La tua mossa è andata a buon fine!");
        if(VerificaVittoria(riga,colonna,Giocatore.segno,CampoDaGioco)==1){
            strcpy(sendline,"Hai vinto!\n");
            return 1;
        }
        if(giocofinito(CampoDaGioco)==1){
            strcpy(sendline,"Pareggio!\n");
            return 2;
        }
        else return 0;
    }
}





int main(int argc, char **argv){

    int sockfd,n;
    struct sockaddr_in6 local_addr, remote_addr;
    socklen_t len = sizeof(struct sockaddr_in6);
    char ipv6_addr[INET6_ADDRSTRLEN];
    char recvline[1000];
    char sendline[1000];
    if(argc < 2){
        printf("Errore. Devi inserire il numero di porta\n");
        return -1;
    }

    sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sockfd < 0){
        printf("Errore nell'apertura di socket\n");
        return -1;
    }

    memset(&local_addr, 0, len);
    local_addr.sin6_family = AF_INET6;
    local_addr.sin6_port = htons(atoi(argv[1]));

    if(bind(sockfd, (struct sockaddr *) &local_addr, len) < 0){
        printf("Errore nell'apertura di bind()\n Utilizza un altro valore\n");
        return -1;
    }



    //FASE DI REGISTRAZIONE
    Player giocatori[2];
    int numerogiocatori = 0;
    while (numerogiocatori < 2)
    {
        n = recvfrom(sockfd,recvline,999,0,(struct sockaddr *)&remote_addr, &len);
        recvline[n] = 0;
        strcpy(giocatori[numerogiocatori].username,strtok(recvline,";"));
        inet_ntop(AF_INET6, &(remote_addr.sin6_addr),giocatori[numerogiocatori].ip,INET6_ADDRSTRLEN);
        giocatori[numerogiocatori].port = ntohs(remote_addr.sin6_port);
        if(numerogiocatori == 0) giocatori[numerogiocatori].segno = 'X';
        else giocatori[numerogiocatori].segno = 'O';
        memset(&sendline,0,1000);
        sprintf(sendline,"Ti sei registrato %s\n",giocatori[numerogiocatori].username);
        sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr*)&remote_addr,sizeof(remote_addr));
        numerogiocatori++;
    }
    
    char **CampoDaGioco = InizioCampoDaGioco();
    int riga;
    int colonna;




    int turno = 0;
    for(;;){
        remote_addr.sin6_port = htons(giocatori[turno].port);
        inet_pton(AF_INET6, giocatori[turno].ip,&remote_addr.sin6_addr);

        PrintCampoDaGioco(sendline,CampoDaGioco);
        sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr*)&remote_addr,sizeof(remote_addr));
        n = recvfrom(sockfd,recvline,999,0,(struct sockaddr *)&remote_addr, &len);
        recvline[n] = 0;
        riga = atoi(strtok(recvline,","));
        colonna = atoi(strtok(NULL,";"));
        n = Mossa(riga,colonna,CampoDaGioco,giocatori[turno],sendline);
        if(n == -1){
            sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr*)&remote_addr,sizeof(remote_addr));
            continue;
        }
        else if(n == 0){
            sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr*)&remote_addr,sizeof(remote_addr));
            turno = (turno +1) %2;
        }
        else if(n == 1){
            sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr*)&remote_addr,sizeof(remote_addr));
            remote_addr.sin6_port = htons(giocatori[(turno+1)%2].port);
            inet_pton(AF_INET6, giocatori[(turno+1)%2].ip,&remote_addr.sin6_addr);
            strcpy(sendline,"Hai perso!\n");
            sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr*)&remote_addr,sizeof(remote_addr));
            break;
        }
        else if(n == 2){
            sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr*)&remote_addr,sizeof(remote_addr));
            remote_addr.sin6_port = htons(giocatori[(turno+1)%2].port);
            inet_pton(AF_INET6, giocatori[(turno+1)%2].ip,&remote_addr.sin6_addr);
            strcpy(sendline,"Pareggio!\n");
            sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr*)&remote_addr,sizeof(remote_addr));
            break;
        }
    }

    close(sockfd);
    return 0;
}