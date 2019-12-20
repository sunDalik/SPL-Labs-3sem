#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include "system_info.h"

struct system_info *sys_info;

void handle_signals(int sig) {
    printf("Signal received: %s\n", strsignal(sig));
    printf("time = %lu\n", sys_info->startup_time);
    printf("pid = %u\n", sys_info->pid);
    printf("uid = %u\n", sys_info->uid);
    printf("gid = %u\n", sys_info->gid);
    printf("average system load in 1min = %f\n", sys_info->sys_loads[0]);
    printf("average system load in 5min = %f\n", sys_info->sys_loads[1]);
    printf("average system load in 15min = %f\n", sys_info->sys_loads[2]);
    printf("Server terminated\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    char *socket_address;
    int can_start = false;

    sys_info = (struct system_info *) malloc(sizeof(struct system_info));

    pid_t pid = getpid();
    uid_t uid = getuid();
    gid_t gid = getgid();
    time_t start_time = time(NULL);

    // parse socket address from cmd args
    int opt = 0;
    while ((opt = getopt(argc, argv, "s:")) != -1) {
        switch (opt) {
            case 's':
                socket_address = optarg;
                can_start = true;
                break;
        }
    }
    if (!can_start) {
        fprintf(stderr, "Usage: %s -s socket_addr\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_address, sizeof(addr.sun_path) - 1);
    unsigned int addr_len = sizeof(struct sockaddr_un);
    bind(fd, (const struct sockaddr *) &addr, addr_len);
    listen(fd, 0);

    printf("Init parameters:\n");
    printf("pid = %u\n", pid);
    printf("uid = %u\n", uid);
    printf("gid = %u\n\n", gid);
    printf("socket name: %s\n\n", socket_address);

    struct sigaction action = {.sa_handler = handle_signals, .sa_flags = 0};
    sigemptyset(&action.sa_mask);
    sigaction(SIGHUP, &action, NULL);
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);
    sigaction(SIGUSR1, &action, NULL);
    sigaction(SIGUSR2, &action, NULL);

    sys_info->pid = pid;
    sys_info->uid = uid;
    sys_info->gid = gid;
    sys_info->startup_time = time(NULL) - start_time;
    getloadavg(sys_info->sys_loads, 3);

    printf("Server is running...\n");
    while (1) {
        // accept client connection
        int cltfd = accept(fd, (struct sockaddr *) &addr, &addr_len);
        // updating current time
        sys_info->startup_time = time(NULL) - start_time;
        // updating average system load
        getloadavg(sys_info->sys_loads, 3);
        // send reply
        write(cltfd, (const void *) sys_info, sizeof(struct system_info));
        close(cltfd);
    }
}