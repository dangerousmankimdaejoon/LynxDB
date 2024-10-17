#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <time.h>

#include "../include/main.h"

#define TEST 5031

/*
 * create_log_directory - logging function
 * @log_dir: $SD_HOME/log
 * 
 * If not exists log directory create 
 */
void create_log_directory(const char *log_dir) {
    struct stat st = {0};
    if (stat(log_dir, &st) == -1) {
        mkdir(log_dir, 0750);
    }
}

/*
 * open_log_file - logging function
 * 
 * Create log file
 */
FILE* open_log_file() {
    
    const char *sd_home = getenv("LX_HOME");

    if (sd_home == NULL) {
        printf("LX_HOME path not exists.\n");
        return 1;
    }

    char log_dir[256];
    snprintf(log_dir, sizeof(log_dir), "%s/log", sd_home);

    create_log_directory(log_dir);

    char date[16];

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(date, sizeof(date), "%Y%m%d", t);
    
    char log_filename[256];
    snprintf(log_filename, sizeof(log_filename), "%s/lynxdb_%s.log", log_dir, date);

    FILE *log_file = fopen(log_filename, "a");
    if (log_file == NULL) {
        perror("Error opening log file");
        exit(1);
    }
    return log_file;
}

/*
 * lx_log - logging function
 * @level: log level
 * @format: log format
 * 
 * logging message
 */
void lx_log(const char *level, const char *format, ...) {
    FILE *log_file = open_log_file();

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);

    va_list args;
    va_start(args, format);
    fprintf(log_file, "[%s] %s: ", timestamp, level);
    vfprintf(log_file, format, args);
    fprintf(log_file, "\n");
    va_end(args);

    fclose(log_file);
}

/*
 * lx_info - logging function
 * @format: log format
 * 
 * logging message (INFO)
 */
void lx_info(const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    lx_log("INFO", format, args);

    va_end(args);
}

/*
 * create_lx_db_session - create process pool
 * @server_sock: server socket file descriptor
 * @*argv[]: arguments array
 * 
 * Change the name of the session process
 */
void create_lx_db_session(int server_sock, char *argv[]) {
    for (int i = 0; i < PROCESS_POOL_SIZE; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) {
            // Change the name of the session process
            char session_nm[50];
            snprintf(session_nm, sizeof(session_nm), "lynx_ses_%03d", i);
            
            strncpy(argv[0], session_nm, strlen(session_nm));
            argv[0][strlen(session_nm)] = '\0';
            prctl(PR_SET_NAME, session_nm, 0, 0, 0);

            while (1) {
                int client_sock;
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);

                client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
                if (client_sock < 0) {
                    perror("accept failed");
                    exit(EXIT_FAILURE);
                }

                handle_client(client_sock);
            }
            exit(0);
        }
    }
}

/*
 * handle_client - client handler
 * @client_sock: client socket file descriptor
 * 
 * test client handler
 */
void handle_client(int client_sock) {
    char query_command[COMMAND_SIZE];
    char query_result[RESULT_SIZE];

    while(1) {
        recv(client_sock, query_command, sizeof(query_command), 0);
        
        lx_log("INFO", "Received command : %s", query_command);

        if(strncmp(query_command, "insert", 6) == 0) {

            FILE* file = fopen("/home/seer/lynx/build/test.db", "a");

            if (file == NULL) {
                lx_log("ERROR", "Error opening file");
                exit(1);
            }

            fprintf(file, substring(query_command, 7, -1));
            fclose(file);

            send(client_sock, "Insert complete", strlen("Insert complete"), 0);
        }
        else if(strncmp(query_command, "select", 6) == 0) {
            FILE* file = fopen("/home/seer/lynx/build/test.db", "r");

            if (file == NULL) {
                perror("Error opening file");
                exit(1);
            }

            while (fgets(query_result, RESULT_SIZE, file) != NULL) {
                send(client_sock, query_result, strlen(query_result), 0);
            }

            fclose(file);
        }
        else if(strncmp(query_command, "quit", 6) == 0 || strncmp(query_command, "exit", 6) == 0) {
            
        }
        else {
            printf("command error");
            send(client_sock, "command error", strlen("command error"), 0);
        }
    }
}

int main(int argc, char *argv[]) {
    
    printf("start lynxDB\n");

    pid_t pid;

    pid = fork();

    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }

    umask(0);
    chdir("/");

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Change the name of the primary process
    const char *prm_process_nm = "lynx_prm";
    
    strncpy(argv[0], prm_process_nm, strlen(prm_process_nm));

    argv[0][strlen(prm_process_nm)] = '\0';

    prctl(PR_SET_NAME, prm_process_nm, 0, 0, 0);

    lx_log("INFO", "*******************");
    lx_log("INFO", "   Starting lynx   ");
    lx_log("INFO", "*******************");

    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int opt = 1;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    lx_log("INFO", "server_sock : %d", server_sock);

    if (server_sock == -1) {
        lx_log("ERROR", "Socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        lx_log("ERROR", "setsockopt");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        lx_log("ERROR", "Bind failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    if (listen(server_sock, 3) < 0) {
        lx_log("ERROR", "Listen failed");
        exit(EXIT_FAILURE);
    }

    // create process pool
    create_lx_db_session(server_sock, argv);

    while (1) {
        sleep(10);
        lx_info("Daemon is running");
    }

    close(server_sock);
    return 0;
}