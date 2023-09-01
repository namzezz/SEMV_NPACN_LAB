#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

void print_permutations(char *str, int l, int r) {
    if (l == r) {
        printf("%s\n", str);
        return;
    }
    
    for (int i = l; i <= r; i++) {
        // Swap characters
        char temp = str[l];
        str[l] = str[i];
        str[i] = temp;

        print_permutations(str, l + 1, r);

        // Revert the swap
        temp = str[l];
        str[l] = str[i];
        str[i] = temp;
    }
}

int main() {
    int sockfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    char buffer[256];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error binding");
        exit(1);
    }

    clilen = sizeof(cli_addr);

    while (1) {
        bzero(buffer, 256);
        recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&cli_addr, &clilen);
        
        printf("Received: %s\n", buffer);
        
        printf("All permutations:\n");
        print_permutations(buffer, 0, strlen(buffer) - 1);

        sendto(sockfd, "Permutations printed.", 21, 0, (struct sockaddr *)&cli_addr, clilen);
    }

    return 0;
}
