#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "system_info.h"

bool parse_int(const char *str, int *var) {
    char *end;
    *var = (int) strtol(str, &end, 10);
    if (end == str) return false;
    return true;
}

int main(int argc, char *argv[]) {
    struct system_info *sys_info = NULL;

    // setup client from args
    int opt = 0;
    int can_start = false;
    while ((opt = getopt(argc, argv, "v:m:q:")) != -1) {
        switch (opt) {
            case 'v':
                printf("system-v mode\n");
                // getting system_info structure from system V memory segment
                int sysVMemID;
                parse_int(optarg, &sysVMemID);
                printf("id = %u\n\n", sysVMemID);
                sys_info = (struct system_info *) shmat(sysVMemID, NULL, 0);
                can_start = true;
                break;
            case 'm':
                printf("mmap mode\n");
                printf("name = %s\n\n", optarg);
                // opening file and getting system_info structure from it
                int mmapFD = open(optarg, O_RDWR, 0644);  // open file
                sys_info = (struct system_info *) mmap(NULL, sizeof(struct system_info),
                                                       PROT_READ, MAP_SHARED, mmapFD, 0); // map file to memory
                can_start = true;
                break;
            case 'q':
                printf("message-queue mode\n");
                // initialize message queue
                int msgQID;
                parse_int(optarg, &msgQID);
                printf("id = %u\n\n", msgQID);
                // send message
                msgbuf_t msg;
                msg.mtype = MSGTYPE_QUERY;
                msgsnd(msgQID, &msg, 0, 0);
                // receive message
                msgrcv(msgQID, &msg, sizeof(struct system_info), MSGTYPE_REPLY, 0);
                sys_info = (struct system_info *) malloc(sizeof(struct system_info));
                memcpy(sys_info, msg.mtext, sizeof(struct system_info));
                can_start = true;
                break;
        }
    }

    if (!can_start) {
        fprintf(stderr, "Usage: %s [-v] [-m] [-q]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("Data received:\n");
    printf("time = %lu\n", sys_info->startup_time);
    printf("pid = %u\n", sys_info->pid);
    printf("uid = %u\n", sys_info->uid);
    printf("gid = %u\n", sys_info->gid);
    printf("average system load in 1min = %f\n", sys_info->sys_loads[0]);
    printf("average system load in 5min = %f\n", sys_info->sys_loads[1]);
    printf("average system load in 15min = %f\n", sys_info->sys_loads[2]);
}