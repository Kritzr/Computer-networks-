#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#define PORT 2100
#define BUFFER_SIZE 1024

void handle_client(int client_socket);
void send_response(int client_socket, const char *response);

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    printf("Krithika-2022503035-FTP");
    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket to address and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Start listening
    if (listen(server_socket, 5) == -1) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("FTP server listening on port %d...\n", PORT);

    while (1) {
        // Accept client connections
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len)) == -1) {
            perror("Accept failed");
            continue;
        }

        printf("Client connected.\n");

        // Handle client in a separate function
        handle_client(client_socket);

        // Close client connection
        close(client_socket);
        printf("Client disconnected.\n");
    }

    // Close server socket
    close(server_socket);
    return 0;
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    // Send initial greeting
    send_response(client_socket, "220 Welcome to the FTP server.\r\n");

    while (1) {
        // Read client command
        memset(buffer, 0, BUFFER_SIZE);
        bytes_read = read(client_socket, buffer, BUFFER_SIZE - 1);

        if (bytes_read <= 0) {
            if (bytes_read < 0) {
                perror("Read failed");
            }
            break; // Exit loop on error or client disconnect
        }

        buffer[bytes_read] = '\0';
        printf("Received command: %s", buffer);

        // Parse command
        if (strncmp(buffer, "USER", 4) == 0) {
            send_response(client_socket, "331 Username okay, need password.\r\n");
        } else if (strncmp(buffer, "PASS", 4) == 0) {
            send_response(client_socket, "230 User logged in, proceed.\r\n");
        } else if (strncmp(buffer, "LIST", 4) == 0) {
            // Simulate directory listing
            send_response(client_socket, "150 Opening data connection.\r\n");
            send_response(client_socket, "226 Transfer complete.\r\n");
        } else if (strncmp(buffer, "QUIT", 4) == 0) {
            send_response(client_socket, "221 Goodbye.\r\n");
            break; // Exit loop on QUIT command
        } else {
            send_response(client_socket, "500 Unknown command.\r\n");
        }
    }
}

void send_response(int client_socket, const char *response) {
    if (write(client_socket, response, strlen(response)) == -1) {
        perror("Write failed");
    }
}

