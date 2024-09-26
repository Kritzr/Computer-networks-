#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 2000
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[BUFFER_SIZE];
    ssize_t n;

    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

   
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 

    
    while (1) {
        
        printf("Client: ");
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            perror("fgets failed");
            break;
        }

       
        if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
            perror("Send failed");
            break;
        }

        
        n = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0, NULL, NULL);
        if (n < 0) {
            perror("Receive failed");
            break;
        }

       
        buffer[n] = '\0';
        printf("Server: %s", buffer);

       
        if (strncmp(buffer, "END", 3) == 0) {
            break;
        }
    }

   
    close(sockfd);
    return 0;
}


