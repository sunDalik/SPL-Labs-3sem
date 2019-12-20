#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/mman.h>
#include <signal.h>
#include <fcntl.h>
#include "system_info.h"

#define UNDEFINED_MODE 0
#define SYSV_SHM_MODE 1
#define MMAP_MODE 2
#define MSGQ_MODE 3

struct system_info *sys_info;

int sysVMemID;
void *sysVMemPointer;

int mmapFD;
char filename[12];

int msgQID;

void shutdown_server() {
    printf("\nServer terminated\n");
    if (sysVMemPointer) {
        shmdt(sysVMemPointer);
        printf("System V shared memory pointer released\n");
    } else if (mmapFD) {
        remove(filename);
        printf("mmap file deleted\n");
    }
    exit(0);
}

int main(int argc, char *argv[]) {
    pid_t pid = getpid();
    uid_t uid = getuid();
    gid_t gid = getgid();
    time_t start_time = time(NULL);
    signal(SIGINT, shutdown_server);

    int opt = 0;
    int run_mode = UNDEFINED_MODE;
    while ((opt = getopt(argc, argv, "vmq")) != -1) {
        switch (opt) {
            case 'v':
                run_mode = SYSV_SHM_MODE;
                //initialize shared memory segment
                sysVMemID = shmget(IPC_PRIVATE, sizeof(struct system_info), IPC_CREAT | 0644);
                sysVMemPointer = shmat(sysVMemID, NULL, 0); //locate the shared memory segment somewhere
                sys_info = (struct system_info *) sysVMemPointer;
                break;

            case 'm':
                run_mode = MMAP_MODE;
                sprintf(filename, "%d", pid);
                mmapFD = open(filename, O_RDWR | O_CREAT, 0644); //create new file with the name of the PID
                ftruncate(mmapFD, sizeof(struct system_info)); //set file size
                sys_info = (struct system_info *) mmap(NULL, sizeof(struct system_info), PROT_WRITE | PROT_READ,
                                                       MAP_SHARED, mmapFD, 0); //map file to memory
                break;

            case 'q':
                run_mode = MSGQ_MODE;
                //initialize message queue
                msgQID = msgget(IPC_PRIVATE, IPC_CREAT | 0644);
                sys_info = (struct system_info *) malloc(sizeof(struct system_info));
                break;
        }
    }
    if (run_mode == UNDEFINED_MODE) {
        fprintf(stderr, "Usage: %s {-v|-m|-q}\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("Init parameters:\n");
    printf("pid = %u\n", pid);
    printf("uid = %u\n", uid);
    printf("gid = %u\n\n", gid);

    switch (run_mode) {
        case SYSV_SHM_MODE:
            printf("System V shared memory mode\n");
            printf("Created System V shared memory segment:\n");
            printf("id = %u\n", sysVMemID);
            printf("size = %lu bytes\n\n", sizeof(struct system_info));
            break;
        case MMAP_MODE:
            printf("mmap mode\n");
            printf("Created new file:\n");
            printf("name = %s\n", filename);
            printf("size = %lu bytes\n\n", sizeof(struct system_info));
            break;
        case MSGQ_MODE:
            printf("System V message queue mode\n");
            printf("Created new message queue:\n");
            printf("id = %u\n\n", msgQID);
            break;
    }

    sys_info->pid = pid;
    sys_info->uid = uid;
    sys_info->gid = gid;
    sys_info->startup_time = time(NULL) - start_time;
    getloadavg(sys_info->sys_loads, 3);

    printf("Server is running...\n");
    while (1) {
        if (run_mode == MSGQ_MODE) {
            //receive message
            msg_t msg;
            if (msgrcv(msgQID, &msg, 0, MSGTYPE_QUERY, 0) != -1) {
                sys_info->startup_time = time(NULL) - start_time;
                getloadavg(sys_info->sys_loads, 3);

                //send reply
                msg.mtype = MSGTYPE_REPLY;
                memcpy(msg.mtext, sys_info, sizeof(struct system_info));
                msgsnd(msgQID, &msg, sizeof(struct system_info), 0);
            }
        } else {
            //update the info every second
            sleep(1);
            sys_info->startup_time = time(NULL) - start_time;
            getloadavg(sys_info->sys_loads, 3);
        }
    }
}