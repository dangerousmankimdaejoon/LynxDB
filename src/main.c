#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <time.h>

#define LOG_DIR "log"
#define PORT 5031
#define BLOCK_SIZE 8192

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

    printf("log dir : %s\n", log_dir);

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
 * log_message - logging function
 * @level: log level
 * @format: log format
 * 
 * logging message
 */
void log_message(const char *level, const char *format, ...) {
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

void handle_client(int client_sock) {
    char command[BLOCK_SIZE];
    char buffer[BLOCK_SIZE];

    recv(client_sock, command, sizeof(command), 0);
    log_message("INFO", "Received command:");

    if (strncmp(command, "write", 5) == 0) {
        FILE* file = fopen("daemon_datafile.db", "wb");
        if (file == NULL) {
            perror("Error opening file");
            exit(1);
        }

        strcpy(buffer, "Data written by daemon.\n");
        fwrite(buffer, sizeof(char), strlen(buffer), file);
        fclose(file);
        
        send(client_sock, "Write complete", strlen("Write complete"), 0);
    } 
    else if (strncmp(command, "read", 4) == 0) {
        FILE* file = fopen("daemon_datafile.db", "rb");
        if (file == NULL) {
            perror("Error opening file");
            exit(1);
        }

        fread(buffer, sizeof(char), BLOCK_SIZE, file);
        fclose(file);

        send(client_sock, buffer, strlen(buffer), 0);
    }

    close(client_sock);
}

int main() {
    
    printf("start\n");

    pid_t pid;
    pid = fork();
    
    printf("pid : %d\n", pid);
    
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

    freopen("/home/seer/lynxdb/build/lynxdb_stdout.log", "a", stdout);
    freopen("/home/seer/lynxdb/build/lynxdb_stderr.log", "a", stderr);
    
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    log_message("INFO", "[Starting lynx]");

    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int opt = 1;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    log_message("INFO", "server_sock : %d", server_sock);

    if (server_sock == -1) {
        log_message("ERROR", "Socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        log_message("ERROR", "setsockopt");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        log_message("ERROR", "Bind failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    if (listen(server_sock, 3) < 0) {
        log_message("ERROR", "Listen failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_len);
        if (client_sock == -1) {
            log_message("ERROR", "Accept failed");
            exit(EXIT_FAILURE);
        }

        pid = fork();
            
        if (pid == -1) {
            log_message("ERROR", "Fork failed");
            close(client_sock);
            continue;
        }
        
        if (pid == 0) {
            close(server_sock);
            handle_client(client_sock);
        } else {
            close(client_sock);
            waitpid(-1, NULL, WNOHANG);
        }
    }

    close(server_sock);
    return 0;
}