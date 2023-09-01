#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


void searchAndSendCount(int client_socket, FILE* file, char* searchString){
char line[256];
int count=0;

while(fgets(line, sizeof(line), file)!=NULL){
if(strstr(line, searchString)!=NULL){
count++;
}
}

send(client_socket, &count, sizeof(count), 0);
}


void replaceInFile(FILE* file, const char* str1, const char* str2){
char buffer[4096];
size_t str1_len = strlen(str1);
size_t str2_len = strlen(str2);

FILE* temp = tmpfile(); //temporary file

while(fgets(buffer, sizeof(buffer), file)){

char* pos;
          while((pos=strstr(buffer,str1))!=NULL){

            fwrite(buffer, 1, pos - buffer, temp); // Write before the match
            fwrite(str2, 1, str2_len, temp);        // Write the replacement
            buffer[strlen(buffer) - str1_len] = '\0'; // Remove the replaced string
            pos += str1_len;                       // Move past the replaced string
            strcpy(buffer, pos);                   // Continue with the rest of the line
        }
        fwrite(buffer, 1, strlen(buffer), temp);   // Write remaining buffer to temp file
    }
}
rewind(temp); //move to the beginning
rewind(file);//move to the beginning of the original file
while (fgets(buffer, sizeof(buffer) , temp)){
fputs(buffer,file); //copy contents from temp file to original file
}
fclose(temp); //close and delete temp file
}



void reorderFile(FILE* file) {
    char lines[1000][256];
    int lineCount = 0;
    
    // Read lines from the file
    while (fgets(lines[lineCount], sizeof(lines[0]), file) != NULL)) {
        lineCount++;
    }
    
    // Sort lines using bubble sort (ASCII comparison)
    for (int i = 0; i < lineCount - 1; i++) {
        for (int j = 0; j < lineCount - i - 1; j++) {
            if (strcmp(lines[j], lines[j + 1]) > 0) {
                char temp[256];
                strcpy(temp, lines[j]);
                strcpy(lines[j], lines[j + 1]);
                strcpy(lines[j + 1], temp);
            }
        }
    }
    
    // Write sorted lines back to the file
    rewind(file);
    for (int i = 0; i < lineCount; i++) {
        fputs(lines[i], file);
    }
}



int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);
    
    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    // Bind the socket
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    
    // Listen for incoming connections
    listen(server_socket, 5);
    
    printf("Server listening...\n");
    
    // Accept connection
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_size);
    printf("Connected to client\n");
    
    // Handle client requests
    char filename[256];
    recv(client_socket, filename, sizeof(filename), 0);
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        send(client_socket, "File not present", sizeof("File not present"), 0);
    } else {
        send(client_socket, "File present", sizeof("File present"), 0);



while (1) {
        int option;
        recv(client_socket, &option, sizeof(option), 0);
        
        if (option == 4) {
            break;
        }
        
        switch (option) {
            case 1: {
                char searchString[256];
                recv(client_socket, searchString, sizeof(searchString), 0);
                
                FILE* file = fopen(filename, "r");
                if (file != NULL) {
                    searchAndSendCount(client_socket, file, searchString);
                    fclose(file);
                }
                break;
            }
            
            case 2: {
                char str1[256], str2[256]               recv(client_socket, str1, sizeof(str1), 0);
                recv(client_socket, str2, sizeof(str2), 0);
             }
                break;
            }
            
            case 3: {
                FILE* file = fopen(filename, "r+");
                if (file != NULL) {
                    reorderFile(file);
                    fclose(file);
                    send(client_socket, "File reordered", sizeof("File reordered"), 0);
                }
                break;
            }
            
            default:
                break;
        }
    }
    
  fclose(file);
    }
    
    // Close sockets
    close(client_socket);
    close(server_socket);
    
    return 0;
}
