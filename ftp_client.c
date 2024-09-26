#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 2100
#define BUFFER_SIZE 1024

void send_command(int socket, const char *command);
void receive_response(int socket);

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char server_ip[] = "127.0.0.1"; // Change this to the IP address of the FTP server
    char command[BUFFER_SIZE];
     printf("Krithika - 2022503035-FTP");
    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server.\n");

    // Main command loop
    while (1) {
        // Get user command
        printf("Enter FTP command: ");
        if (fgets(command, sizeof(command), stdin) == NULL) {
            perror("Input error");
            break;
        }
        command[strcspn(command, "\n")] = '\0'; // Remove newline character

        // Send command to server
        send_command(sockfd, command);

        // Receive and print server response
        receive_response(sockfd);

        // Exit on QUIT command
        if (strncmp(command, "QUIT", 4) == 0) {
            break;
        }
    }

    // Close socket
    close(sockfd);
    printf("Connection closed.\n");
    return 0;
}

void send_command(int socket, const char *command) {
    char buffer[BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "%s\r\n", command);
    if (write(socket, buffer, strlen(buffer)) == -1) {
        perror("Write failed");
    }
}

void receive_response(int socket) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    // Read response from server
    memset(buffer, 0, BUFFER_SIZE);
    bytes_read = read(socket, buffer, BUFFER_SIZE - 1);
    if (bytes_read <= 0) {
        if (bytes_read < 0) {
            perror("Read failed");
        } else {
            printf("Server closed the connection.\n");
        }
        return;
    }

    buffer[bytes_read] = '\0';
    printf("Server response: %s", buffer);
}

