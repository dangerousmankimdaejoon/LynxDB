#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVER_PORT 5031
#define BLOCK_SIZE 8192

int main() {
    int sock = 0;
    struct sockaddr_in server_addr;
    char buffer[BLOCK_SIZE] = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to the server failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    char* command = "write";
    send(sock, command, strlen(command), 0);
    read(sock, buffer, BLOCK_SIZE);
    printf("Server: %s\n", buffer);

    command = "read";
    send(sock, command, strlen(command), 0);
    read(sock, buffer, BLOCK_SIZE);
    printf("Server: %s\n", buffer);

    close(sock);

    return 0;
}