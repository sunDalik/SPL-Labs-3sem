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
char sock_addr[12];

void handle_signals(int sig) {
    printf("\nSignal received: %s\n", strsignal(sig));
    printf("time = %lu\n", sys_info->run_time);
    printf("pid = %u\n", sys_info->pid);
    printf("uid = %u\n", sys_info->uid);
    printf("gid = %u\n", sys_info->gid);
    printf("average system load in 1min = %f\n", sys_info->sys_loads[0]);
    printf("average system load in 5min = %f\n", sys_info->sys_loads[1]);
    printf("average system load in 15min = %f\n", sys_info->sys_loads[2]);
    printf("Server terminated\n");
    remove(sock_addr);
    exit(EXIT_SUCCESS);
}

int main() {
    sys_info = (struct system_info *) malloc(sizeof(struct system_info));
    pid_t pid = getpid();
    uid_t uid = getuid();
    gid_t gid = getgid();
    time_t start_time = time(NULL);

    sprintf(sock_addr, "%d", pid);
    int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, sock_addr, sizeof(addr.sun_path) - 1);
    unsigned int addr_len = sizeof(struct sockaddr_un);
    bind(sock_fd, (const struct sockaddr *) &addr, addr_len);
    listen(sock_fd, 0);

    printf("Init parameters:\n");
    printf("pid = %u\n", pid);
    printf("uid = %u\n", uid);
    printf("gid = %u\n\n", gid);
    printf("socket name: %s\n\n", sock_addr);

    sys_info->pid = pid;
    sys_info->uid = uid;
    sys_info->gid = gid;
    sys_info->run_time = time(NULL) - start_time;
    getloadavg(sys_info->sys_loads, 3);

    struct sigaction action = {.sa_handler = handle_signals, .sa_flags = 0};
    sigaction(SIGHUP, &action, NULL);
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);
    sigaction(SIGUSR1, &action, NULL);
    sigaction(SIGUSR2, &action, NULL);

    printf("Server is running...\n");
    while (1) {
        int client_fd = accept(sock_fd, (struct sockaddr *) &addr, &addr_len);
        sys_info->run_time = time(NULL) - start_time;
        getloadavg(sys_info->sys_loads, 3);
        write(client_fd, (const void *) sys_info, sizeof(struct system_info));
        close(client_fd);
    }
}