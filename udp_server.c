#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 2000
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t cliaddr_len;
    char buffer[BUFFER_SIZE];
    ssize_t n;

    printf("2022503035- UDP CHAT SYSTEM");
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

   
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server is ready to receive messages...\n");

    
    while (1) {
        cliaddr_len = sizeof(cliaddr);
       
       
        n = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr *)&cliaddr, &cliaddr_len);
        if (n < 0) {
            perror("Receive failed");
            break;
        }

       
        buffer[n] = '\0';
        printf("Client: %s", buffer);

        
        if (strncmp(buffer, "END", 3) == 0) {
            strcpy(buffer, "BYE");
            sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&cliaddr, cliaddr_len);
            break;
        }

       
        printf("Server: ");
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            perror("fgets failed");
            break;
        }

        
        if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&cliaddr, cliaddr_len) < 0) {
            perror("Send failed");
            break;
        }
    }

  
    close(sockfd);
    return 0;
}

