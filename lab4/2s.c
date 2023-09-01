#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_registration(int server_socket, struct sockaddr_in client_addr, socklen_t addr_size) {
    // Simulate fetching registration details from a database
    char name[] = "John Doe";
    char address[] = "123 Main St, City";
    int pid = getpid();
    
    char response[BUFFER_SIZE];
    snprintf(response, BUFFER_SIZE, "Name: %s\nAddress: %s\nPID: %d", name, address, pid);
    
    sendto(server_socket, response, strlen(response), 0, (struct sockaddr*)&client_addr, addr_size);
}

void handle_name(int server_socket, struct sockaddr_in client_addr, socklen_t addr_size) {
    // Simulate fetching student enrollment details from a database
    char dept[] = "Computer Science";
    char semester[] = "3rd";
    char section[] = "A";
    char courses[] = "Course 1, Course 2";
    int pid = getpid();
    
    char response[BUFFER_SIZE];
    snprintf(response, BUFFER_SIZE, "Dept: %s\nSemester: %s\nSection: %s\nCourses: %s\nPID: %d", dept, semester, section, courses, pid);
    
    sendto(server_socket, response, strlen(response), 0, (struct sockaddr*)&client_addr, addr_size);
}

void handle_subject(int server_socket, struct sockaddr_in client_addr, socklen_t addr_size) {
    // Simulate fetching subject marks from a database
    char subject_marks[] = "Subject A: 85, Subject B: 92";
    int pid = getpid();
    
    char response[BUFFER_SIZE];
    snprintf(response, BUFFER_SIZE, "Subject Marks: %s\nPID: %d", subject_marks, pid);
    
    sendto(server_socket, response, strlen(response), 0, (struct sockaddr*)&client_addr, addr_size);
}

int main() {
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);
    
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    
    while (1) {
        int option;
        recvfrom(server_socket, &option, sizeof(option), 0, (struct sockaddr*)&client_addr, &addr_size);
        
        switch (option) {
            case 1:
                handle_registration(server_socket, client_addr, addr_size);
                break;
            case 2:
                handle_name(server_socket, client_addr, addr_size);
                break;
            case 3:
                handle_subject(server_socket, client_addr, addr_size);
                break;
            default:
                break;
        }
    }
    
    return 0;
}
