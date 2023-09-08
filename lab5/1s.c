#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_CLIENTS 2

void *handle_client(void *arg);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int client_count = 0;

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8888);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_socket, 5) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port 8888\n");

    pthread_t client_threads[MAX_CLIENTS];
    struct sockaddr_in client_addrs[MAX_CLIENTS];

    while (1) {
        if (client_count >= MAX_CLIENTS) {
            printf("Terminating session for all clients\n");
            for (int i = 0; i < client_count; i++) {
                pthread_join(client_threads[i], NULL);
            }
            break;
        }

        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket == -1) {
            perror("Accept failed");
            continue;
        }

        pthread_t client_thread;
        pthread_create(&client_thread, NULL, handle_client, (void *)&client_socket);
        client_threads[client_count] = client_thread;
        client_addrs[client_count] = client_addr;
        client_count++;
    }
    close(server_socket);
    return 0;
}

void *handle_client(void *arg) {
    int client_socket = *((int *)arg);
    char buffer[1024];
    
    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);

    pthread_mutex_lock(&mutex);
    if (strcmp(buffer, "Institute Of") == 0) {
        FILE *fp = fopen("manipal.txt", "a");
        if (fp) {
            fprintf(fp, "%s\n", buffer);
            fclose(fp);
        }
        printf("Received '%s' from client\n", buffer);
    }

    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    
    if (strcmp(buffer, "Technology") == 0) {
        FILE *fp = fopen("manipal.txt", "a");
        if (fp) {
            fprintf(fp, "%s\n", buffer);
            fclose(fp);
        }
        printf("Received '%s' from client\n", buffer);
    }
    pthread_mutex_unlock(&mutex);

    printf("Manipal Institute of Technology\n");
    FILE *fp = fopen("manipal.txt", "r");
    if (fp) {
        char content[1024];
        while (fgets(content, sizeof(content), fp) != NULL) {
            printf("%s", content);
        }
        fclose(fp);
    }

    close(client_socket);
    return NULL;
}
