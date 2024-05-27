#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#define N 6
#define M 7

typedef struct Player{
    char username[20];
    char ip[20];
    int porta;
    int segno;
}Player;



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
void PrintCampoDaGioco(int **Matrice, char line[]){//server
    strcpy(line,"");
    char aux[500] = "";
    for(int i = 0; i < M;i++) {
        sprintf(aux," %d  ",i);
        strcat(line,aux);
    }
    strcat(line,"\n");

    for(int i = 0; i < N; i++){
        for(int j = 0; j < M ; j++){
            sprintf(aux,"[%d] ",Matrice[i][j]);
            strcat(line,aux);
        }
        strcat(line,"\n");
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
    int i = 0;
    if(Num_Giocatore == 0){
        while(i < N && Matrice[i][colonna] == 0) i++;
        Matrice[--i][colonna] = 1;        
    }
    if(Num_Giocatore == 1){
        while(i < N && Matrice[i][colonna] == 0) i++;
        Matrice[--i][colonna] = 2;        
    }
    printf("mossa : %d\n",colonna);
    if(VerificaVittoria(Num_Giocatore,i,colonna,Matrice)) exit(-1);
    if(FineGioco(Matrice)) {
        printf("Gioco Terminato!\n");
        exit(-1);    
    };
}







int main(int argc, char **argv){
    int sockfd,n;
    struct sockaddr_in local_addr; 
    struct sockaddr_in remote_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    
    char recvline[1000];
    char sendline[1000];

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



    Player Giocatori[2];
    int numerogiocatori = 0;
    //FASE DI REGISTRAZIONE DEI GIOCATORI
    while (numerogiocatori < 2)
    {
        n = recvfrom(sockfd,recvline,999,0,(struct sockaddr*)&remote_addr,&len);
        recvline[n] = 0;
        strcpy(Giocatori[numerogiocatori].username,strtok(recvline,","));
        strcpy(Giocatori[numerogiocatori].ip,inet_ntoa(remote_addr.sin_addr));
        Giocatori[numerogiocatori].porta = ntohs(remote_addr.sin_port);
        Giocatori[numerogiocatori].segno = numerogiocatori;
        numerogiocatori++;
        strcpy(sendline,"Ti seri registrato correttamente\n");
        sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr*)&remote_addr,sizeof(remote_addr));
    }
    
    //INIZIO FASE CREAZIONE DEL CAMPO DA GIOCO
    int turno = 0;
    int mossa;
    int **CampoDaGioco = InizioCampoDaGioco();
    for(;;){
        printf("E' il turno di %s\n",Giocatori[turno].username);
        inet_pton(AF_INET,Giocatori[turno].ip,&remote_addr.sin_addr);
        remote_addr.sin_port = htons(Giocatori[turno].porta);
        PrintCampoDaGioco(CampoDaGioco,sendline);
        printf("Sto inviando %s",sendline);
        sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr*)&remote_addr,sizeof(remote_addr));
        n = recvfrom(sockfd,recvline,999,0,(struct sockaddr*)&remote_addr,&len);
        recvline[n] = 0;
        mossa = atoi(recvline);
        if(VerificaMossa(mossa,CampoDaGioco)==1){
            strcpy(sendline,"Errore, mossa non valida!\n");
            sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr*)&remote_addr,sizeof(remote_addr));
            continue;
        }
        MossaGiocatore(Giocatori[turno].segno,mossa,CampoDaGioco);
        strcpy(sendline,"La tua mossa è andata a buon fine!\n");
        sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr*)&remote_addr,sizeof(remote_addr));
        turno = (turno+1)%2;
    }

    close(sockfd);
    return 0;
}