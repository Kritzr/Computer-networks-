#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT_NUM 3035
#define MAX_BUFFER 1024

int main() {
    int client_sock;
    struct sockaddr_in server_address;
    char recv_buffer[MAX_BUFFER];
    ssize_t received_bytes;

    printf("Krithika- 2022503035- TCP DATE AND TIME CLIENT\n");

    // Create a socket
    if ((client_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    // Define the server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT_NUM);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // Local server IP address

    // Connect to the server
    if (connect(client_sock, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Connection error");
        close(client_sock);
        exit(EXIT_FAILURE);
    }

    // Receive data from the server
    received_bytes = recv(client_sock, recv_buffer, MAX_BUFFER - 1, 0);
    if (received_bytes == -1) {
        perror("Receive error");
        close(client_sock);
        exit(EXIT_FAILURE);
    }

    // Null-terminate and print the received data
    recv_buffer[received_bytes] = '\0';
    printf("Server response: %s", recv_buffer);

    // Close the socket
    close(client_sock);
    return 0;
}

