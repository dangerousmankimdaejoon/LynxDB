#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 5031
#define BLOCK_SIZE 8192

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BLOCK_SIZE] = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
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