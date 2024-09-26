#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX 30
#define PORT 2100

int main() {
    int sersoc, conn;
    char str[MAX];
    pid_t pid;
    time_t ticks;
    socklen_t clilen;
    struct sockaddr_in servaddr, cliaddr;

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((sersoc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket Error");
        exit(EXIT_FAILURE);
    }

    if (bind(sersoc, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind Error");
        close(sersoc);
        exit(EXIT_FAILURE);
    }

    if (listen(sersoc, 10) < 0) {
        perror("Listen Error");
        close(sersoc);
        exit(EXIT_FAILURE);
    }

    for (;;) {
        clilen = sizeof(cliaddr);
        conn = accept(sersoc, (struct sockaddr *)&cliaddr, &clilen);
        if (conn < 0) {
            perror("Accept Error");
            continue;
        }

        if ((pid = fork()) == 0) {
            close(sersoc);
            ticks = time(NULL);
            strcpy(str, ctime(&ticks));
            if (write(conn, str, strlen(str)) < 0) {
                perror("Write Error");
                close(conn);
                exit(EXIT_FAILURE);
            }
            close(conn);
            exit(EXIT_SUCCESS);
        }
        close(conn);
    }

    close(sersoc);
    return 0;
}

