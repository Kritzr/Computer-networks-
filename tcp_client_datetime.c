#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#define CLI_PORT 3000
#define BUFF_SIZE 30

int main(int argc, char **argv) {
    int clisoc, re;
    char recbuff[BUFF_SIZE];
    struct sockaddr_in cliaddr;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <server-ip>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(CLI_PORT);
    cliaddr.sin_addr.s_addr = inet_addr(argv[1]);

    if ((clisoc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket Error");
        exit(EXIT_FAILURE);
    }

    if (connect(clisoc, (struct sockaddr *)&cliaddr, sizeof(cliaddr)) < 0) {
        perror("Connect Error");
        close(clisoc);
        exit(EXIT_FAILURE);
    }

    if ((re = read(clisoc, recbuff, sizeof(recbuff) - 1)) < 0) {
        perror("Read Error");
        close(clisoc);
        exit(EXIT_FAILURE);
    }

    recbuff[re] = '\0';
    printf("The Current Date and Time : %s\n", recbuff);
    close(clisoc);
    return 0;
}

