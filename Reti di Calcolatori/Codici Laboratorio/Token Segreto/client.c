#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
    int sockfd, n;
    struct sockaddr_in6 local_addr, remote_addr;
    socklen_t len = sizeof(struct sockaddr_in6);
    char buffer[1000];
    char messaggio[900];
    char ipv6_addr[INET6_ADDRSTRLEN];

    if (argc < 4)
    {
        printf("Errore! Inserisci tutti i parametri\n");
        return -1;
    }
    if ((sockfd = socket(AF_INET6, SOCK_DGRAM, 0)) < 0)
    {
        printf("Errore nell'apertura di socket\n");
        return -1;
    }
    memset(&remote_addr, 0, len);
    remote_addr.sin6_family = AF_INET6;
    inet_pton(AF_INET6, argv[1], &(remote_addr.sin6_addr));
    remote_addr.sin6_port = htons(atoi(argv[2]));
    int scelta;
    char username[50];
    char password[50];
    int porta;
    char *esito;
    long int token;


    do
    {
        printf("Cosa vuoi fare?\n1 Registrati\n2 Effettua il login\n");
        fgets(buffer, sizeof(buffer), stdin);
        scelta = atoi(buffer);

        switch (scelta)
        {
        case 1:
            printf("Inserisci il tuo username\n");
            fgets(username, sizeof(username), stdin);
            username[strcspn(username, "\n")] = 0; 

            printf("Inserisci la tua password\n");
            fgets(password, sizeof(password), stdin);
            password[strcspn(password, "\n")] = 0; 

            printf("Inserisci il numero di porta\n");
            fgets(buffer, sizeof(buffer), stdin);
            porta = atoi(buffer);

            if (porta != atoi(argv[3]))
                exit(-1);

            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "Reg,%s,%s,%d;", username, password, porta);
            sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&remote_addr, len);
            n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&remote_addr, &len);
            buffer[n] = 0;
            printf("Ho ricevuto: %s\n", buffer);
            esito = strtok(buffer, ",");
            break;
        case 2:
            printf("Inserisci il tuo username\n");
            fgets(username, sizeof(username), stdin);
            username[strcspn(username, "\n")] = 0; 

            printf("Inserisci la tua password\n");
            fgets(password, sizeof(password), stdin);
            password[strcspn(password, "\n")] = 0;

            printf("Inserisci il numero di porta\n");
            fgets(buffer, sizeof(buffer), stdin);
            porta = atoi(buffer);

            if (porta != atoi(argv[3]))
                exit(-1);

            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "Log,%s,%s,%d;", username, password, porta);
            sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&remote_addr, len);
            n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&remote_addr, &len);
            buffer[n] = 0;
            printf("Ho ricevuto: %s\n", buffer);
            esito = strtok(buffer, ",");
            break;
        default:
            printf("Non hai inserito una scelta valida\n");
            break;
        }
    } while (strcmp(esito, "Ti sei loggato correttamente") != 0 && strcmp(esito, "Ti sei registrato correttamente") != 0);

    token = atol(strtok(NULL, ";"));
    printf("Il tuo token è: %ld\n", token);

    if (fork())
    {
        close(sockfd);
        if ((sockfd = socket(AF_INET6, SOCK_DGRAM, 0)) < 0)
        {
            printf("Errore nell'apertura di socket\n");
            return -1;
        }
        memset(&local_addr, 0, len);
        local_addr.sin6_family = AF_INET6;
        local_addr.sin6_port = htons(atoi(argv[3]));

        if (bind(sockfd, (struct sockaddr *)&local_addr, len) < 0)
        {
            printf("Errore nella bind\n");
            return -1;
        }

        for (;;)
        {
            n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&remote_addr, &len);
            buffer[n] = 0;
            inet_ntop(AF_INET6, &(remote_addr.sin6_addr), ipv6_addr, INET6_ADDRSTRLEN);
            printf("IP = %s, Port = %d, buffer = %s\n", ipv6_addr, ntohs(remote_addr.sin6_port), buffer);
        }
        close(sockfd);
        return 0;
    }
    else
    {
        memset(&remote_addr, 0, len);
        remote_addr.sin6_family = AF_INET6;
        inet_pton(AF_INET6, argv[1], &(remote_addr.sin6_addr));
        remote_addr.sin6_port = htons(atoi(argv[2]));
        for (;;)
        {
            printf("Cosa vuoi mandare?\n");
            fgets(messaggio, sizeof(messaggio), stdin);
            messaggio[strcspn(messaggio, "\n")] = 0;

            snprintf(buffer, sizeof(buffer), "Msg,%ld,%s;", token, messaggio);
            printf("%s\n", buffer);
            sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&remote_addr, len);
        }
        close(sockfd);
        return 0;
    }
}
