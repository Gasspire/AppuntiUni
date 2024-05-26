#include <stdio.h>
#include <stdlib.h>
#include <string.h>




int main(int argc,char *argv[]){
    char Giocatori[2][10];
    for (int i = 0; i < 2; i++)
    {
        printf("Inserisci roba\n");
        scanf("%s",Giocatori[i]);

    }

    for (int i = 0; i < 2; i++)
    {
        printf("Nomi: %s\n",Giocatori[i]);
    }
    
    char sex[20];
    strcpy(sex,Giocatori[0]);
    printf("%s",sex);



}