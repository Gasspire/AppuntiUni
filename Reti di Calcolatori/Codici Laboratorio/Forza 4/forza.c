#include <stdio.h>
#include <stdlib.h>

#define N 6
#define M 5

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


//Funzione che verifica la vittoria ad ogni mossa, prende in input il giocatore per capire quali segni verificare e la posizione dell'ultima mossa
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
        scanf("%d",&colonna);
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




int main(){
    int **CampoDaGioco = InizioCampoDaGioco();
    int turno = 1;
    int mossa;
    while(1){
        printf("Turno del giocatore %d \n", turno);
        printf("Inserisci la colonna in cui vuoi inserire il tuo gettone! \n");
        scanf("%d",&mossa);
        MossaGiocatore(turno,mossa,CampoDaGioco);
        turno = (turno+1) % 2;
    }
    return 0;
}