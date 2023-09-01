#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024

void handle_registration(int client_socket) {
    // Simulate fetching registration details from a database
    char name[] = "John Doe";
    char address[] = "123 Main St, City";
    int pid = getpid();
    
    char response[BUFFER_SIZE];
    snprintf(response, BUFFER_SIZE, "Name: %s\nAddress: %s\nPID: %d", name, address, pid);
    send(client_socket, response, strlen(response), 0);
}

void handle_name(int client_socket) {
    // Simulate fetching student enrollment details from a database
    char dept[] = "Computer Science";
    char semester[] = "3rd";
    char section[] = "A";
    char courses[] = "Course 1, Course 2";
    int pid = getpid();
    
    char response[BUFFER_SIZE];
    snprintf(response, BUFFER_SIZE, "Dept: %s\nSemester: %s\nSection: %s\nCourses: %s\nPID: %d", dept, semester, section, courses, pid);
    send(client_socket, response, strlen(response), 0);
}

void handle_subject(int client_socket) {
    // Simulate fetching subject marks from a database
    char subject_marks[] = "Subject A: 85, Subject B: 92";
    int pid = getpid();
    
    char response[BUFFER_SIZE];
    snprintf(response, BUFFER_SIZE, "Subject Marks: %s\nPID: %d", subject_marks, pid);
    send(client_socket, response, strlen(response), 0);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);
    
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, MAX_CLIENTS);
    
    while (1) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_size);
        
        if (fork() == 0) {
            close(server_socket);
            
            int option;
            recv(client_socket, &option, sizeof(option), 0);
            
            switch (option) {
                case 1:
                    handle_registration(client_socket);
                    break;
                case 2:
                    handle_name(client_socket);
                    break;
                case 3:
                    handle_subject(client_socket);
                    break;
                default:
                    break;
            }
            
            close(client_socket);
            exit(0);
        }
        
        close(client_socket);
    }
    
    return 0;
}
