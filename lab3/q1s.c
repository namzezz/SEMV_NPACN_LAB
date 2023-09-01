#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

int main() {
    int sockfd, newsockfd;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int pid;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    // Initialize server address structure
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error binding");
        exit(1);
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    printf("Server listening on port %d\n", PORT);

    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("Error on accept");
            exit(1);
        }

        pid = fork();  // Create a child process

        if (pid < 0) {
            perror("Error on fork");
            exit(1);
        }

        if (pid == 0) { // Child process
            close(sockfd);  // Close parent's socket

            while (1) {
                bzero(buffer, 256);
                recv(newsockfd, buffer, 255, 0);
                printf("Client: %s\n", buffer);
                printf("Enter response: ");
                fgets(buffer, 255, stdin);
                send(newsockfd, buffer, strlen(buffer), 0);
            }
            close(newsockfd);
            exit(0);
        } else { // Parent process
            close(newsockfd);  // Close child's socket
        }
    }

    close(sockfd);
    return 0;
}
