#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    
    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // Connect to server

      connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
 
// Send filename to server
    char filename[256];
    printf("Enter filename: ");
    scanf("%s", filename);
    send(client_socket, filename, sizeof(filename), 0);
    
    char response[256];
    recv(client_socket, response, sizeof(response), 0);
    
    if (strcmp(response, "File not present") == 0) {
        printf("File not present on server.\n");
        close(client_socket);
        return 0;
    } else {
        printf("File present on server.\n");

  while (1) {
        int option;
        printf("Menu:\n1. Search\n2. Replace\n3. Reorder\n4. Exit\nEnter option: ");
        scanf("%d", &option);
        send(client_socket, &option, sizeof(option), 0);
        
        if (option == 4) {
            break;
        }
        
        switch (option) {
            case 1: {
                char searchString[256];
                printf("Enter search string: ");
                scanf("%s", searchString);
                send(client_socket, searchString, sizeof(searchString), 0);
                
                int count;
                recv(client_socket, &count, sizeof(count), 0);
                printf("String occurred %d times in the file.\n", count);
                break;
            }
            
            case 2: {
                char str1[256], str2[256];
                printf("Enter string to replace: ");
                scanf("%s", str1);
                printf("Enter replacement string: ");
                scanf("%s", str2);
                send(client_socket, str1, sizeof(str1), 0);
                send(client_socket, str2, sizeof(str2), 0);
                
                char response[256];
                recv(client_socket, response, sizeof(response), 0);
                printf("%s\n", response);
                break;
            }
            
            case 3: {
                // Send option
                send(client_socket, "Reorder", sizeof("Reorder"), 0);
                
                char response[256];
                recv(client_socket, response, sizeof(response), 0);
                printf("%s\n", response);
                break;
            }
            
            default:
                break;
        }
    }   





}

 close(client_socket);
    
    return 0;
}
