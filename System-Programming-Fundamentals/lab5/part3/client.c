#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdbool.h>
#include "system_info.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s socket_addr\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *socket_address = argv[1];
    struct system_info sys_info;
    printf("Connecting to socket %s\n\n", socket_address);

    int fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    struct sockaddr_un addr;
    addr.sun_family = AF_LOCAL;
    strncpy(addr.sun_path, socket_address, sizeof(addr.sun_path) - 1);
    unsigned int addr_len = sizeof(struct sockaddr_un);
    connect(fd, (const struct sockaddr *) &addr, addr_len);
    read(fd, &sys_info, sizeof(struct system_info));

    printf("Data received:\n");
    printf("time = %lu\n", sys_info.startup_time);
    printf("pid = %u\n", sys_info.pid);
    printf("uid = %u\n", sys_info.uid);
    printf("gid = %u\n", sys_info.gid);
    printf("average system load in 1min = %f\n", sys_info.sys_loads[0]);
    printf("average system load in 5min = %f\n", sys_info.sys_loads[1]);
    printf("average system load in 15min = %f\n", sys_info.sys_loads[2]);

    close(fd);
    return EXIT_SUCCESS;
}