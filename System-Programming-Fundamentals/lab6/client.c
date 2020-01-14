#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFSIZE 4096

int main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s host port paths...\n", argv[0]);
        return EXIT_FAILURE;
    }

    struct addrinfo hints = {
            .ai_family = AF_INET, .ai_socktype = SOCK_STREAM, .ai_protocol = IPPROTO_TCP
    };
    struct addrinfo *addr;
    int err;
    if ((err = getaddrinfo(argv[1], argv[2], &hints, &addr)) != 0) {
        fprintf(stderr, "Failed to resolve host: %s\n", gai_strerror(err));
        return EXIT_FAILURE;
    }

    // getaddrinfo() returns a list of suitable addresses that match given node and service
    int sock_fd = -1;
    for (struct addrinfo *a = addr; a != NULL; a = a->ai_next) {
        if ((sock_fd = socket(a->ai_family, a->ai_socktype, a->ai_protocol)) == -1) continue;
        if (connect(sock_fd, a->ai_addr, a->ai_addrlen) == 0) break;
        else sock_fd = close(sock_fd) - 1;
    }
    if (sock_fd <= -1) {
        fprintf(stderr, "Unable to connect to %s:%s\n", argv[1], argv[2]);
        return EXIT_FAILURE;
    }
    freeaddrinfo(addr);

    for (int i = 3; i < argc; ++i) {
        write(sock_fd, argv[i], strlen(argv[i]));
        write(sock_fd, "\r\n", 2);
    }
    write(sock_fd, "\r\n", 2);

    char buf[BUFSIZE];
    int bytes_read;
    while ((bytes_read = read(sock_fd, buf, BUFSIZE)) > 0) {
        write(STDOUT_FILENO, buf, bytes_read);
    }

    close(sock_fd);
    return EXIT_SUCCESS;
}