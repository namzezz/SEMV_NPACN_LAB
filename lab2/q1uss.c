// udp_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void searchAndSendCount(int client_socket, FILE* file, char* searchString, struct sockaddr_in client_addr, socklen_t addr_size) {
    char line[256];
    int count = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, searchString) != NULL) {
            count++;
        }
    }

    sendto(client_socket, &count, sizeof(count), 0, (struct sockaddr*)&client_addr, addr_size);
}

int main() {
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);

    // Create socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    printf("Server listening...\n");

    // Handle client requests
    while (1) {
        char filename[256];
        int option;
        char buffer[256];

        recvfrom(server_socket, filename, sizeof(filename), 0, (struct sockaddr*)&client_addr, &addr_size);
        FILE* file = fopen(filename, "r");
        if (file == NULL) {
            strcpy(buffer, "File not present");
            sendto(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, addr_size);
        } else {
            strcpy(buffer, "File present");
            sendto(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, addr_size);

            while (1) {
                recvfrom(server_socket, &option, sizeof(option), 0, (struct sockaddr*)&client_addr, &addr_size);

                if (option == 4) {
                    fclose(file);
                    break;
                }

                switch (option) {
                    case 1: {
                        char searchString[256];
                        recvfrom(server_socket, searchString, sizeof(searchString), 0, (struct sockaddr*)&client_addr, &addr_size);
                        searchAndSendCount(server_socket, file, searchString, client_addr, addr_size);
                        break;
                    }

                    // Other cases here

                    default:
                        break;
                }
            }
        }
    }

    close(server_socket);

    return 0;
}
