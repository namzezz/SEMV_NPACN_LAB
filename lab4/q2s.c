#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/dns_socket"
#define BUFFER_SIZE 256

typedef struct {
    char domain[BUFFER_SIZE];
    char ip_address[BUFFER_SIZE];
} DNSRecord;

void handle_dns_request(int client_socket, const char* query) {
    FILE* db_file = fopen("database.txt", "r");
    if (db_file == NULL) {
        perror("Failed to open database file");
        return;
    }

    char response[BUFFER_SIZE] = "Not Found";
    char line[BUFFER_SIZE];
    while (fgets(line, sizeof(line), db_file)) {
        DNSRecord record;
        sscanf(line, "%s %s", record.domain, record.ip_address);
        if (strcmp(query, record.domain) == 0) {
            snprintf(response, BUFFER_SIZE, "IP Address: %s", record.ip_address);
            break;
        }
    }
    fclose(db_file);

    send(client_socket, response, strlen(response), 0);
}

int main() {
    unlink(SOCKET_PATH);

    int server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_un server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(server_socket);
        return 1;
    }

    if (listen(server_socket, 5) == -1) {
        perror("Listen failed");
        close(server_socket);
        return 1;
    }

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == -1) {
            perror("Accept failed");
            continue;
        }

        char query[BUFFER_SIZE];
        recv(client_socket, query, sizeof(query), 0);

        handle_dns_request(client_socket, query);

        close(client_socket);
    }

    close(server_socket);
    unlink(SOCKET_PATH);

    return 0;
}
