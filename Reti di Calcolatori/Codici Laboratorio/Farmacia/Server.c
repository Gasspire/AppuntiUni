/* Sample TCP server */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

typedef struct farmac
{
    int id;
    char nome[20];
    int quantita;
} Farmaco;

void stampalista(char sendline[])
{
    FILE *db = NULL;
    db = fopen("registro.txt", "r+");
    if (db == NULL)
        exit(-1);
    strcpy(sendline, "");
    char line[500];

    while (fgets(line, 500, db) != NULL)
    {
        strcat(sendline, line);
    }
    fclose(db);
    return;
}

void acquisto(int id, int quantita, char sendline[])
{
    FILE *db = NULL;
    FILE *temp = NULL;
    Farmaco aux;
    char line[500];
    db = fopen("registro.txt", "r+");
    temp = fopen("temp.txt", "w+");
    if (db == NULL || temp == NULL)
        exit(-1);
    strcpy(sendline, "Farmaco non trovato o quantità insufficente");

    while (fgets(line, 500, db) != NULL)
    {
        aux.id = atoi(strtok(line, ","));
        strcpy(aux.nome, strtok(NULL, ","));
        aux.quantita = atoi(strtok(NULL, ";"));
        if (aux.id == id)
        {
            if (aux.quantita >= quantita)
            {
                aux.quantita = aux.quantita - quantita;
                strcpy(sendline, "Farmaco trovato");
            }
        }
        fprintf(temp, "%d,%s,%d;\n", aux.id, aux.nome, aux.quantita);
    }
    fclose(temp);
    fclose(db);
    remove("registro.txt");
    rename("temp.txt", "registro.txt");
    return;
}

void acquistoindirizzo(int id, int quantita,char indirizzo[], char sendline[])
{
    FILE *db = NULL;
    FILE *temp = NULL;
    FILE *spedizioni = NULL;
    Farmaco aux;
    char line[500];
    db = fopen("registro.txt", "r+");
    temp = fopen("temp.txt", "w+");
    spedizioni = fopen("spedizioni.txt","a+");
    
    if (db == NULL || temp == NULL || spedizioni == NULL) exit(-1);
    strcpy(sendline, "Farmaco non trovato o quantità insufficente");

    while (fgets(line, 500, db) != NULL)
    {
        aux.id = atoi(strtok(line, ","));
        strcpy(aux.nome, strtok(NULL, ","));
        aux.quantita = atoi(strtok(NULL, ";"));
        if (aux.id == id)
        {
            if (aux.quantita >= quantita)
            {
                aux.quantita = aux.quantita - quantita;
                fprintf(spedizioni,"%s,%s,%d;\n",indirizzo,aux.nome,quantita);
                strcpy(sendline, "Farmaco trovato");
            }
        }
        fprintf(temp, "%d,%s,%d;\n", aux.id, aux.nome, aux.quantita);
    }
    fclose(temp);
    fclose(db);
    fclose(spedizioni);
    remove("registro.txt");
    rename("temp.txt", "registro.txt");
    return;
}


int main(int argc, char **argv)
{
    int sockfd, newsockfd, n;
    struct sockaddr_in6 local_addr, remote_addr;
    socklen_t len;
    char recvline[1000];
    char sendline[1000];
    char ipv6_addr[50];
    int IsRemote;
    char indirizzo[50];
    Farmaco aux;

    if (argc < 2)
    {
        printf("Use: server listeing_PORT");
        return 0;
    }

    if ((sockfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0)
    {
        printf("\nErrore nell'apertura del socket");
        return -1;
    }
    memset((char *)&local_addr, 0, sizeof(local_addr));
    local_addr.sin6_family = AF_INET6;
    local_addr.sin6_port = htons(atoi(argv[1]));

    if (bind(sockfd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0)
    {
        printf("\nErrore nel binding. Errore %d \n", errno);
        return -1;
    }

    listen(sockfd, 5);

    char *action;
    for (;;)
    {
        len = sizeof(remote_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&remote_addr, &len);

        if (fork() == 0)
        {
            close(sockfd);
            n = recv(newsockfd, recvline, 999, 0);
            recvline[n] = 0;
            if (strcmp(recvline, "Remoto") == 0) IsRemote = 1;
            if (strcmp(recvline, "Presenza")==0) IsRemote = 0;
            



            for (;;)
            {
                n = recv(newsockfd, recvline, 999, 0);
                if (n == 0)
                    return 0;
                recvline[n] = 0;
                inet_ntop(AF_INET6, &(remote_addr.sin6_addr), ipv6_addr, INET6_ADDRSTRLEN);
                printf("received from %s:%d the following: %s\n", ipv6_addr, ntohs(remote_addr.sin6_port), recvline);

                action = strtok(recvline, ",");
                if (strcmp(action, "Stampa") == 0)
                {
                    stampalista(sendline);
                    send(newsockfd, sendline, strlen(sendline), 0);
                }
                if (strcmp(action, "Acquisto") == 0)
                {
                    if (IsRemote == 0)
                    {
                        aux.id = atoi(strtok(NULL, ","));
                        aux.quantita = atoi(strtok(NULL, ";"));
                        acquisto(aux.id, aux.quantita, sendline);
                        send(newsockfd, sendline, strlen(sendline), 0);
                    }
                    else{
                        aux.id = atoi(strtok(NULL, ","));
                        aux.quantita = atoi(strtok(NULL, ","));
                        strcpy(indirizzo,strtok(NULL,";"));
                        acquistoindirizzo(aux.id, aux.quantita,indirizzo, sendline);
                        send(newsockfd, sendline, strlen(sendline), 0);
                    }
                }
                if (strcmp(action, "Chiusura") == 0)
                    return 0;
            }
            return 0;
        }
        else
            close(newsockfd);
    }
}
