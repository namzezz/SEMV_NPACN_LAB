#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void sortAscendingNumbers(char *str) {
    // Sort numbers in ascending order
    for (int i = 0; i < strlen(str); i++) {
        for (int j = i + 1; j < strlen(str); j++) {
            if (str[i] >= '0' && str[i] <= '9' && str[j] >= '0' && str[j] <= '9') {
                if (str[i] > str[j]) {
                    char temp = str[i];
                    str[i] = str[j];
                    str[j] = temp;
                }
            }
        }
    }
}

void sortDescendingChars(char *str) {
    // Sort characters in descending order
    for (int i = 0; i < strlen(str); i++) {
        for (int j = i + 1; j < strlen(str); j++) {
            if (str[i] >= 'a' && str[i] <= 'z' && str[j] >= 'a' && str[j] <= 'z') {
                if (str[i] < str[j]) {
                    char temp = str[i];
                    str[i] = str[j];
                    str[j] = temp;
                }
            }
        }
    }
}

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
    serv_addr.sin_port = htons(8080);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error binding");
        exit(1);
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    printf("Server listening on port 8080\n");

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

            bzero(buffer, 256);
            recv(newsockfd, buffer, sizeof(buffer), 0);
            printf("Received from client: %s\n", buffer);

            // Create a copy of the buffer for parent process
            char parent_buffer[256];
            strcpy(parent_buffer, buffer);

            // Sort numbers in ascending order
            sortAscendingNumbers(buffer);
            printf("Child Process (PID: %d) - Sorted Numbers in Ascending Order: %s\n", getpid(), buffer);
            send(newsockfd, buffer, strlen(buffer), 0);

            // Sort characters in descending order
            sortDescendingChars(parent_buffer);
            printf("Parent Process (PID: %d) - Sorted Characters in Descending Order: %s\n", getpid(), parent_buffer);
            send(newsockfd, parent_buffer, strlen(parent_buffer), 0);

            close(newsockfd);
            exit(0);
        } else { // Parent process
            close(newsockfd);  // Close child's socket
        }
    }

    close(sockfd);
    return 0;
}
