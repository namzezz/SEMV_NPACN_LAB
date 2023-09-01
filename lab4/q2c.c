#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/dns_socket"
#define BUFFER_SIZE 256

int main() {
    int client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_un server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connect failed");
        close(client_socket);
        return 1;
    }

    char query[BUFFER_SIZE];
    printf("Enter domain name to resolve: ");
    scanf("%s", query);

    send(client_socket, query, strlen(query), 0);

    char response[BUFFER_SIZE];
    recv(client_socket, response, sizeof(response), 0);

    printf("DNS Response: %s\n", response);

    close(client_socket);

    return 0;
}
