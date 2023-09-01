#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    
    int option;
    printf("Select an option:\n1. Registration Number\n2. Name of the Student\n3. Subject Code\n");
    scanf("%d", &option);
    send(client_socket, &option, sizeof(option), 0);
    
    char response[BUFFER_SIZE];
    recv(client_socket, response, sizeof(response), 0);
    printf("Server Response:\n%s\n", response);
    
    close(client_socket);
    
    return 0;
}
