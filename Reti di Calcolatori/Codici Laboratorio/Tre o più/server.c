#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <time.h>

typedef struct player
{
    char username[50];
    char ipv6_addr[50];
    int port;
    int punti;
} Player;

char **InizioCampoDaGioco()
{
    char **Matrice;
    Matrice = (char **)malloc(sizeof(char *) * 6);
    for (int i = 0; i < 6; i++)
    {
        Matrice[i] = (char *)malloc(sizeof(char) * 6);
    }
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            Matrice[i][j] = '-';
        }
    }
    return Matrice;
}

void PrintCampo(char sendline[], char **CampoDaGioco)
{
    memset(sendline, 0, 999);
    char aux[20];
    strcat(sendline, "    0     1     2     3     4     5\n");
    for (int i = 0; i < 6; i++)
    {
        strcat(sendline, "  ___________________________________\n");
        sprintf(aux, "%d ", i);
        strcat(sendline, aux);
        for (int j = 0; j < 6; j++)
        {
            memset(aux, 0, 20);
            sprintf(aux, "| %c | ", CampoDaGioco[i][j]);
            strcat(sendline, aux);
        }
        strcat(sendline, "\n");
    }
}

void SpawnGettoni(char **CampoDaGioco,int gettoni)
{
    char Colori[3] = {'R', 'G', 'B'};
    srand(time(NULL));
    int bool = 1;
    int n = 0;
    int m = 0;
    for (int i = 0; i < gettoni; i++)
    {
        while (bool)
        {
            n = rand() % 6;
            m = rand() % 6;
            if (CampoDaGioco[n][m] == '-') bool = 0;
        }
        bool = 1;
        CampoDaGioco[n][m] = Colori[rand()%3];
    }
}

int verificaMossa(int si,int sj,int di,int dj, char **CampoDaGioco){
    if(di < 0 || di > 5 || si < 0 || si > 5 ) return 0;
    if(dj < 0 || dj > 5 || sj < 0 || sj > 5) return 0;
    if(CampoDaGioco[di][dj] != '-') return 0;
    if(CampoDaGioco[si][sj] == '-') return 0;
    return 1;
}
void Swap(int si,int sj,int di,int dj, char **CampoDaGioco){
    char aux = CampoDaGioco[si][sj];
    CampoDaGioco[si][sj] = CampoDaGioco[di][dj];
    CampoDaGioco[di][dj] = aux;
}
int VerificaOrizzontale(int i, int j, char **CampoDaGioco){
    int conteggio = 0;
    char colore = CampoDaGioco[i][j];
    while(j > -1 && CampoDaGioco[i][j] == colore){
        j--;
    }
    j++;
    while(j<6 && CampoDaGioco[i][j] == colore){
        conteggio++;
        j++;
    }
    j--;
    if(conteggio >= 3){
        while(j > -1 && CampoDaGioco[i][j] == colore){ //eliminiamo
            CampoDaGioco[i][j] = '-';
            j--;
        }
        return conteggio;
    }
    return 0;
}

int VerificaVerticale(int i, int j, char **CampoDaGioco){
    int conteggio = 0;
    char colore = CampoDaGioco[i][j];
    while(i > -1 && CampoDaGioco[i][j] == colore){
        i--;
    }
    i++;
    while(i<6 && CampoDaGioco[i][j] == colore){
        conteggio++;
        i++;
    }
    i--;
    if(conteggio >= 3){
        while(i > -1 && CampoDaGioco[i][j] == colore){ //eliminiamo
            CampoDaGioco[i][j] = '-';
            i--;
        }
        return conteggio;
    }
    return 0;
}

int mossa(int si,int sj,int di,int dj, char** CampoDaGioco){
    int score = 0;
    if(verificaMossa(si,sj,di,dj,CampoDaGioco)==1){
        Swap(si,sj,di,dj,CampoDaGioco);
        score = VerificaOrizzontale(di,dj,CampoDaGioco);
        if(score == 0) score = VerificaVerticale(di,dj,CampoDaGioco);
    }
    else return -1;
    return score;
}

int FineGioco(char**CampoDaGioco){
    for(int i = 0; i<6;i++){
        for(int j = 0; j<6; j++){
            if(CampoDaGioco[i][j]=='-') return 0;
        }
    }
    return 1;
}
int main(int argc, char **argv)
{

    int sockfd, n;
    struct sockaddr_in6 local_addr, remote_addr;
    socklen_t len = sizeof(struct sockaddr_in6);
    char recvline[1000];
    char sendline[1000];
    char ipv6_addr[50];

    if (argc < 2)
    {
        printf("Errore. Devi inserire il numero di porta\n");
        return -1;
    }

    sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        printf("Errore nell'apertura di socket\n");
        return -1;
    }

    memset(&local_addr, 0, len);
    local_addr.sin6_family = AF_INET6;
    local_addr.sin6_port = htons(atoi(argv[1]));

    if (bind(sockfd, (struct sockaddr *)&local_addr, len) < 0)
    {
        printf("Errore nell'apertura di bind()\n Utilizza un altro valore\n");
        return -1;
    }


    // REGISTRAZIONE
    Player giocatori[2];
    int numerogiocatori = 0;
    while (numerogiocatori < 2)
    {
        n = recvfrom(sockfd, recvline, 999, 0, (struct sockaddr *)&remote_addr, &len);
        recvline[n] = 0;
        strcpy(giocatori[numerogiocatori].username, recvline);
        giocatori[numerogiocatori].port = ntohs(remote_addr.sin6_port);
        inet_ntop(AF_INET6, &(remote_addr.sin6_addr), giocatori[numerogiocatori].ipv6_addr, INET6_ADDRSTRLEN);
        giocatori[numerogiocatori].punti = 0;
        memset(sendline, 0, 999);
        printf("%s,%s,%d\n",giocatori[numerogiocatori].username,giocatori[numerogiocatori].ipv6_addr,giocatori[numerogiocatori].port);
        sprintf(sendline, "Ti sei registrato correttamente mastro %s", giocatori[numerogiocatori].username);
        sendto(sockfd, sendline, strlen(sendline) + 1, 0, (struct sockaddr *)&remote_addr, sizeof(remote_addr));
        numerogiocatori++;
    }
    // inizio campo da gioco
    char **CampoDaGioco = InizioCampoDaGioco();
    int gettspawnati = 2;
    SpawnGettoni(CampoDaGioco,gettspawnati);

    int turno = 0;
    int si,sj;
    int di,dj;
    int punti;
    char aux[100];
    for (;;)
    {
        remote_addr.sin6_port = htons(giocatori[turno].port);
        inet_pton(AF_INET6,giocatori[turno].ipv6_addr,&remote_addr.sin6_addr);
        PrintCampo(sendline,CampoDaGioco);
        sprintf(aux,"\nMastro %s hai attualmente %d punti\n",giocatori[turno].username,giocatori[turno].punti);

        strcat(sendline,aux);
        sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr*)&remote_addr,len);
        //aspettiamo la mossa
        n = recvfrom(sockfd, recvline, 999, 0, (struct sockaddr *)&remote_addr, &len);
        recvline[n] = 0;    
        si = atoi(strtok(recvline,","));
        sj = atoi(strtok(NULL,","));
        di = atoi(strtok(NULL,","));
        dj = atoi(strtok(NULL,";"));

        if((punti = mossa(si,sj,di,dj,CampoDaGioco))>=0){
            giocatori[turno].punti += punti;
            memset(sendline,0,999);
            sprintf(sendline,"Hai fatto %d punti",punti);
            sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr*)&remote_addr,len);
            SpawnGettoni(CampoDaGioco,gettspawnati);
            if((rand()%10)<=2) gettspawnati+=2;
            if(giocatori[turno].punti >=10){
                strcpy(sendline,"HAI VINTO!");
                sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr*)&remote_addr,len);
                remote_addr.sin6_port = htons(giocatori[(turno+1)%2].port);
                inet_pton(AF_INET6,giocatori[(turno+1)%2].ipv6_addr,&remote_addr.sin6_addr);
                strcpy(sendline,"HAI PERSO!");
                sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr*)&remote_addr,len);
            }
            if(FineGioco(CampoDaGioco)==1){
                strcpy(sendline,"PAREGGIO!");
                sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr*)&remote_addr,len);
                remote_addr.sin6_port = htons(giocatori[(turno+1)%2].port);
                inet_pton(AF_INET6,giocatori[(turno+1)%2].ipv6_addr,&remote_addr.sin6_addr);
                strcpy(sendline,"PAREGGIO!");
                sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr*)&remote_addr,len);
            }
            turno = (turno+1)%2;
        }
        else{
            memset(sendline,0,999);
            sprintf(sendline,"Errore nella mossa");
            sendto(sockfd,sendline,strlen(sendline)+1,0,(struct sockaddr*)&remote_addr,len);
        }

    }

    close(sockfd);
    return 0;
}