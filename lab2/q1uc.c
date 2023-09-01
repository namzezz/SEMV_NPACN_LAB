// udp_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int client_socket;
    struct sockaddr_in server_addr;

    // Create socket
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Send filename to server
    char filename[256];
    char response[256];
    int option;
    char searchString[256], str1[256], str2[256];

    printf("Enter filename: ");
    scanf("%s", filename);
    sendto(client_socket, filename, sizeof(filename), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    recvfrom(client_socket, response, sizeof(response), 0, NULL, NULL);

    if (strcmp(response, "File not present") == 0) {
        printf("File not present on server.\n");
        close(client_socket);
        return 0;
    } else {
        printf("File present on server.\n");

        while (1) {
            printf("Menu:\n1. Search\n2. Replace\n3. Reorder\n4. Exit\nEnter option: ");
            scanf("%d", &option);
            sendto(client_socket, &option, sizeof(option), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

            if (option == 4) {
                break;
            }

            switch (option) {
                case 1: {
                    printf("Enter search string: ");
                    scanf("%s", searchString);
                    sendto(client_socket, searchString, sizeof(searchString), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

                    int count;
                    recvfrom(client_socket, &count, sizeof(count), 0, NULL, NULL);
                    printf("String occurred %d times in the file.\n", count);
                    break;
                }

               
 // Other cases here
	case 2: {
            printf("Enter string to replace: ");
            scanf("%s", str1);
            printf("Enter replacement string: ");
            scanf("%s", str2);
		char buffer[1256];

	
    // Check if the combined length of str1 and str2 exceeds buffer size - 12 (for "REPLACE  ")
    if (strlen(str1) + strlen(str2) + 12 > sizeof(buffer)) {
        printf("Input strings are too long. Please make them shorter.\n");
        continue; // Skip to the next iteration of the loop
    }

            // Construct the replacement request and send it to the server
            snprintf(buffer, sizeof(buffer), "REPLACE %s %s", str1, str2);
            sendto(client_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

            // Receive the response from the server
            recvfrom(client_socket, response, sizeof(response), 0, NULL, NULL);
            printf("%s\n", response);
            break;
        }


	 case 3: {
                    sendto(client_socket, "Reorder", sizeof("Reorder"), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
                    recvfrom(client_socket, response, sizeof(response), 0, NULL, NULL);
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
