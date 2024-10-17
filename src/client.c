#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>

#include "../include/client.h"

#define TEST 5031

int main() {
    int sock = 0;
    struct sockaddr_in server_addr;
    
    char query_command[COMMAND_SIZE] = {0};
    char query_result[RESULT_SIZE] = {0};
    
    time_t timer = time(NULL);
    struct tm *t = localtime(&timer);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address or Address not supported");
        close(sock);
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to the server failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    char buf_time[30] = {0};
    strftime(buf_time, sizeof(buf_time), "%a %b %d %H:%M:%S %Y", t);
    printf("\nConnect to LynxDB with xsql - %s\n", buf_time);

    printf("Connected to %s:%d\n\n", SERVER_IP, SERVER_PORT);

    while(1) {
        printf("xsql=# ");

        fgets(query_command, sizeof(query_command), stdin);
        query_command[strcspn(query_command, "\n")] = '\0';

        // Send a command to the server
        send(sock, query_command, strlen(query_command), 0);

        int bytes_received = recv(sock, query_result, sizeof(query_result), 0);
        if (bytes_received > 0) {
            query_result[bytes_received] = '\0';
            printf("%s\n", query_result);
        } else {
            printf("No response from server.\n");
        }
    }

    close(sock);

    return 0;
}