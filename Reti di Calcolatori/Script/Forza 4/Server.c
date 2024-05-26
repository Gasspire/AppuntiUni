#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define N 6
#define M 5

//funzioni che servono per la porta
void ControlloInserimentoPorta(int argc){
    if(argc <2 ){
        printf("errore, inserisci la porta da riga di comando\n");
        exit(-1);
    }
}
void ControlloAperturaSocket(int sockfd){
    if(sockfd < 0){
        printf("Errore nell'apertura di socket \n");
        exit(-1);
    }
}
void BindDellaPorta(int sockfd, struct sockaddr_in local_addr,socklen_t len){
    if(bind(sockfd, (struct sockaddr*)&local_addr, len)<0){
        printf("Errore Inserisci un numero di porta differente\n");
        exit(-1);
    }

}

//funzioni che riguardano il campo da gioco
int ** InizioCampoDaGioco(){//server
    int **Matrice =(int **) malloc(sizeof(int *)*N);
    for(int i = 0; i<N; i++){
        Matrice[i] = (int *) malloc(sizeof(int)*M);
    }
    for(int i = 0; i < N; i++){
        for(int j = 0; j< M; j++){
            Matrice[i][j] = 0;
        }
    }
    return Matrice;
}
void PrintCampoDaGioco(int **Matrice){//server
    for(int i = 0; i < M;i++) printf(" %d  ",i);
    printf("\n");

    for(int i = 0; i < N; i++){
        for(int j = 0; j < M ; j++){
            printf("[%d] ",Matrice[i][j]);
        }
        printf("\n");
    }

}
int FineGioco(int **Matrice){//server
    for(int i = 0; i < M; i++){
        if(Matrice[0][i]==0) return 0;
    }
    return 1;
}
int VerificaMossa(int mossa, int **Matrice){//server
    if(mossa > M || mossa < 0) return 1;
    if(Matrice[0][mossa] != 0) return 1;

    return 0;
}
int VerificaOrizzontale(int segno, int i,int j,int **Matrice){
    int conta = 0;
        while(j>=0 && j<M && Matrice[i][j] == segno){
            j--;
        }
        j++;//uscendo dal ciclo arriviamo al punto in cui prima c'era l'ultimo segno del giocatore
        while(Matrice[i][j] == segno && conta<4){
            j++;
            conta++;
        }
        if(conta == 4){
            return 1;
        }
    return 0;
}
int VerificaVerticale(int segno, int i,int j,int **Matrice){
    int conta = 0;
        while(i>=0 && i<N && Matrice[i][j] == segno){
            i++;
        }
        i--;//uscendo dal ciclo arriviamo al punto in cui prima c'era l'ultimo segno del giocatore
        while(Matrice[i][j] == segno && conta<4){
            i--;
            conta++;
        }
        if(conta == 4){
            return 1;
        }
    return 0;
}
int VerificaDiaDX(int segno, int i,int j,int **Matrice){
    int conta = 0;
        while(i>=0 && i<N && j>=0 && j<M && Matrice[i][j] == segno){
            i++;
            j--;
        }
        i--;
        j++;
        while(i>=0 && i<N && j>=0 && j<M && Matrice[i][j] == segno && conta<4){
            i--;
            j++;
            conta++;
        }
        if(conta == 4){
            return 1;
        }
    return 0;
}
int VerificaDiaSX(int segno, int i,int j,int **Matrice){
    int conta = 0;
        while(i>=0 && i<N && j>=0 && j<M && Matrice[i][j] == segno){
            i++;
            j++;
        }
        i--;
        j--;
        while(i>=0 && i<N && j>=0 && j<M && Matrice[i][j] == segno && conta<4){
            i--;
            j--;
            conta++;
        }
        if(conta == 4){
            return 1;
        }
    return 0;
}
int VerificaVittoria(int Num_Giocatore,int i, int j,int **Matrice){
    if(VerificaOrizzontale(Num_Giocatore+1,i,j,Matrice)){
        printf("HAI VINTO!");
        return 1;
    }
    if(VerificaVerticale(Num_Giocatore+1,i,j,Matrice)){
        printf("HAI VINTO!");
        return 1;
    }
    if(VerificaDiaDX(Num_Giocatore+1,i,j,Matrice)){
        printf("HAI VINTO!");
        return 1;
    }
    if(VerificaDiaSX(Num_Giocatore+1,i,j,Matrice)){
        printf("HAI VINTO!");
        return 1;
    }
    return 0;
}
void MossaGiocatore(int Num_Giocatore, int colonna, int **Matrice){
    while(VerificaMossa(colonna,Matrice)){
        printf("Errore, mossa non valida \n");
        printf("Per favore, inserisci un valore valido \n");
        return;
    }
    int i = 0;
    if(Num_Giocatore == 0){
        while(i < N && Matrice[i][colonna] == 0) i++;
        Matrice[--i][colonna] = 1;        
    }
    if(Num_Giocatore == 1){
        while(i < N && Matrice[i][colonna] == 0) i++;
        Matrice[--i][colonna] = 2;        
    }

    PrintCampoDaGioco(Matrice);
    if(VerificaVittoria(Num_Giocatore,i,colonna,Matrice)) exit(-1);
    if(FineGioco(Matrice)) {
        printf("Gioco Terminato!\n");
        exit(-1);    
    };
}

void RegistraGiocatore(char Giocatori[][20],char *IP_Player){
    int i = 0;
    while (i<2 && strcmp(Giocatori[i],"inizio")!=0)
    {
        if(strcmp(Giocatori[i],IP_Player)==0){
            printf("Giocatore gia' registrato\n");
            return;
        }
        i++;
    }
    if(i == 2){
        printf("Completi \n");
        return;
    }
    strcpy(Giocatori[i],IP_Player);
    return;
}



int main(int argc,char **argv){
    ControlloInserimentoPorta(argc);

    int turno = 0; //semaforo che gestisce le richieste, se il semaforo non è a 1 nella persona interessata, allora viene buttato.
    char Giocatori[2][20]; //assegnamo gli ip dei giocatori al numero di giocatore
    for (int i = 0; i < 2; i++)
    {
        strcpy(Giocatori[i],"inizio");
    }
    
    
    int sockfd;
    struct sockaddr_in local_addr, remote_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    char buffer[100];

    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    ControlloAperturaSocket(sockfd);
    
    memset(&remote_addr,0,len);
    memset(&local_addr,0,len);

    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(atoi(argv[1]));

    BindDellaPorta(sockfd,local_addr,len);

    //Adesso il server è pronto a ricevere messaggi
    //inizializziamo tutto il campo da gioco

    int **CampoDaGioco = InizioCampoDaGioco();
    char *IP_Player;
    int mossa;
    for(;;){
        mossa = 0;
        
        recvfrom(sockfd, buffer, 99, 0, (struct sockaddr*)&remote_addr,&len);
        printf("Ricevuto da %s : %s\n",inet_ntoa(remote_addr.sin_addr),buffer);
        IP_Player = inet_ntoa(remote_addr.sin_addr);

        if(strcmp(buffer,"R")==0){
            RegistraGiocatore(Giocatori,IP_Player);
            printf("Ho registrato il giocatore %s\n",IP_Player);
        }
        else if(strcmp(IP_Player,Giocatori[turno])==0){
            mossa = atoi(buffer);
            MossaGiocatore(turno,mossa,CampoDaGioco);
            turno = (turno +1)%2;
        }  
    }

    close(sockfd);    

}