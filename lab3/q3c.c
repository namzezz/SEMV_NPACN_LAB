#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    server = gethostbyname("localhost");
    if (server == NULL) {
        fprintf(stderr, "Error, no such host\n");
        exit(1);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error connecting");
        exit(1);
    }

    printf("Enter an alphanumeric string: ");
    bzero(buffer, BUFFER_SIZE);
    fgets(buffer, BUFFER_SIZE - 1, stdin);
    
    send(sockfd, buffer, strlen(buffer), 0);

    bzero(buffer, BUFFER_SIZE);
    recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
    printf("Child Process Result (Sorted Numbers in Ascending Order, PID: %d): %s\n", getpid(), buffer);

    bzero(buffer, BUFFER_SIZE);
    recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
    printf("Parent Process Result (Sorted Characters in Descending Order, PID: %d): %s\n", getpid(), buffer);

    close(sockfd);
    return 0;
}
