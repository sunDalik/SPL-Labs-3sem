#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <signal.h>
#include <netdb.h>

#define BUFSIZE 4096

const char INVALID_REQUEST_ERROR[] = "Invalid request!\nYou must send at least one path.\nPaths must be on separate lines terminated by CRLF.\nRequest must be terminated with an empty line.\r\n";

typedef struct {
    pthread_t worker;
    int busy;
    int client_fd;
    int id;
} server_thread;

char *read_request(int fd, int *request_len) {
    char *request = NULL;
    *request_len = 0;

    char buf[BUFSIZE];
    int bytes_read;
    while ((bytes_read = read(fd, buf, BUFSIZE)) > 0) {
        request = realloc(request, *request_len + bytes_read);
        memcpy(request + *request_len, buf, bytes_read);
        *request_len += bytes_read;

        if (*request_len >= 2 && request[*request_len - 2] == '\r' && request[*request_len - 1] == '\n') {
            if (*request_len == 2) {
                write(fd, INVALID_REQUEST_ERROR, sizeof(INVALID_REQUEST_ERROR) - 1);
                free(request);
                return NULL;
            }
            if (*request_len > 4 && request[*request_len - 4] == '\r' && request[*request_len - 3] == '\n') {
                return request;
            }
        }
    }

    write(fd, INVALID_REQUEST_ERROR, sizeof(INVALID_REQUEST_ERROR) - 1);
    free(request);
    return NULL;
}

void *worker(void *thread_info) {
    server_thread *meta = (server_thread *) thread_info;
    while (1) {
        while (1) {
            if (meta->busy && meta->client_fd >= 0) break;
            usleep(5000);
        }

        int client_fd = meta->client_fd;
        printf("Thread #%d: client %d connected.\n", meta->id, client_fd);

        //dprintf(client_fd, "Hello client...\r\n");
        //sleep(10);

        int request_len;
        char *paths = read_request(client_fd, &request_len);
        if (paths == NULL) {
            close(client_fd);
            return NULL;
        }

        char *path_start = paths;
        for (char *path_end = paths; path_end < paths + request_len; ++path_end) {
            if (*path_end == '\r') {
                *path_end = '\0';

                errno = 0;
                DIR *dir = opendir(path_start);
                if (errno != 0) {
                    dprintf(client_fd, "Failed to open %s: %s\r\n\r\n", path_start, strerror(errno));
                    path_end += 2;
                    path_start = path_end;
                    continue;
                }

                dprintf(client_fd, "Contents of %s:\r\n", path_start);

                struct dirent *ent;
                while ((ent = readdir(dir)) != NULL)
                    dprintf(client_fd, "%s\r\n", ent->d_name);

                dprintf(client_fd, "\r\n");
                closedir(dir);

                path_end += 2;
                path_start = path_end;
            }
        }

        close(client_fd);
        meta->client_fd = -1;
        meta->busy = false;
    }
}

bool parse_ushort(const char *str, unsigned short *val) {
    errno = 0;
    char *endptr;
    *val = strtoul(str, &endptr, 10);
    return *str != '-' && errno == 0 && endptr != str && *endptr == '\0';
}

server_thread *get_free_thread(server_thread *thread_pool, int thread_pool_size) {
    for (int i = 0; i < thread_pool_size; i++) {
        if (!thread_pool[i].busy) {
            thread_pool[i].busy = true;
            return &thread_pool[i];
        }
    }
    return NULL;
}

int main(int argc, char **argv) {
    // get port number from args
    unsigned short port;
    if (argc < 2 || !parse_ushort(argv[1], &port)) {
        fprintf(stderr, "Usage: %s port [thread_count]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // get worker count from args
    unsigned short thread_count = 2;
    if (argc >= 3 && !parse_ushort(argv[2], &thread_count)) {
        fprintf(stderr, "Usage: %s port [thread_count]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // init worker pool
    server_thread thread_pool[thread_count];
    for (int i = 0; i < thread_count; ++i) {
        thread_pool[i].id = i;
        thread_pool[i].busy = false;
        thread_pool[i].client_fd = -1;
        pthread_create(&thread_pool[i].worker, NULL, worker, (void *) &thread_pool[i]);
    }

    // init server socket
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    // this is necessary if we restart the server at the same address
    const int optVal = 1;
    const socklen_t optLen = sizeof(optVal);
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (void *) &optVal, optLen);

    // bind socket to the address
    struct sockaddr_in srv_addr = {
            .sin_family = AF_INET,
            .sin_addr = {
                    .s_addr = htonl(INADDR_ANY)
            },
            .sin_port = htons(port)
    };
    bind(sock_fd, (struct sockaddr *) &srv_addr, sizeof(struct sockaddr_in));
    listen(sock_fd, 16);
    printf("Listening on %d\n", port);

    // main loop
    while (1) {
        server_thread *free_thread;
        if ((free_thread = get_free_thread(thread_pool, (int) thread_count)) != NULL) {
            int client_fd = accept(sock_fd, NULL, NULL);
            free_thread->client_fd = client_fd;
        }
        usleep(1000);
    }
}