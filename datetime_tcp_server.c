#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT_NUM 3035
#define MAX_BUFFER 1024

void process_client(int client_sock);

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_address, client_address;
    socklen_t address_size = sizeof(client_address);

    printf("Krithika- 2022503035- TCP DATE AND TIME SERVER\n");

    // Create a socket
    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    // Define the server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT_NUM);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(server_sock, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Bind error");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_sock, 5) == -1) {
        perror("Listen error");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    printf("Server is ready and listening on port %d...\n", PORT_NUM);

    while (1) {
        if ((client_sock = accept(server_sock, (struct sockaddr *)&client_address, &address_size)) == -1) {
            perror("Accept error");
            continue;
        }

        printf("Client connected\n");

        process_client(client_sock);

        close(client_sock);
        printf("Client disconnected\n");
    }

    close(server_sock);
    return 0;
}

void process_client(int client_sock) {
    char buffer[MAX_BUFFER];
    time_t now;
    struct tm *time_info;
    int sent_bytes;

    time(&now);
    time_info = localtime(&now);

    strftime(buffer, MAX_BUFFER, "Current date and time: %Y-%m-%d %H:%M:%S\n", time_info);

    sent_bytes = send(client_sock, buffer, strlen(buffer), 0);
    if (sent_bytes == -1) {
        perror("Send error");
    }
}

