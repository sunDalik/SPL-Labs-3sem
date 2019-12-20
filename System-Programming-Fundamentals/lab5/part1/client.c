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

int main(int argc, char *argv[]) {
    struct system_info *sys_info = NULL;
    int can_start = false;

    int opt = 0;
    while ((opt = getopt(argc, argv, "v:m:q:")) != -1) {
        switch (opt) {
            case 'v':
                printf("System V shared memory mode\n");
                int sysVMemID = (int) strtol(optarg, NULL, 10);
                printf("id = %u\n\n", sysVMemID);
                sys_info = (struct system_info *) shmat(sysVMemID, NULL, 0);
                can_start = true;
                break;
            case 'm':
                printf("mmap mode\n");
                printf("name = %s\n\n", optarg);
                int mmapFD = open(optarg, O_RDONLY, 0644);
                sys_info = (struct system_info *) mmap(NULL, sizeof(struct system_info),
                                                       PROT_READ, MAP_SHARED, mmapFD, 0);
                can_start = true;
                break;
            case 'q':
                printf("Message queue mode\n");
                int msgQID = (int) strtol(optarg, NULL, 10);
                printf("id = %u\n\n", msgQID);
                msg_t msg;
                msg.mtype = MSGTYPE_QUERY;
                msgsnd(msgQID, &msg, 0, 0);

                msgrcv(msgQID, &msg, sizeof(struct system_info), MSGTYPE_REPLY, 0);
                sys_info = (struct system_info *) malloc(sizeof(struct system_info));
                memcpy(sys_info, msg.mtext, sizeof(struct system_info));
                can_start = true;
                break;
        }
    }

    if (!can_start) {
        fprintf(stderr, "Usage: %s {-v|-m|-q} id \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("Data received:\n");
    printf("time = %lu\n", sys_info->run_time);
    printf("pid = %u\n", sys_info->pid);
    printf("uid = %u\n", sys_info->uid);
    printf("gid = %u\n", sys_info->gid);
    printf("average system load in 1min = %f\n", sys_info->sys_loads[0]);
    printf("average system load in 5min = %f\n", sys_info->sys_loads[1]);
    printf("average system load in 15min = %f\n", sys_info->sys_loads[2]);
    return EXIT_SUCCESS;
}

