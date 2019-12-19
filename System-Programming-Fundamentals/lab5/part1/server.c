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

#define UNDEFMODE 0
#define SYSVMODE 1
#define MMAPMODE 2
#define MSGQMODE 3

int mmapFD;     // mmap file descriptor
char filename[12];  // all numbers are representable by str[12]
struct system_info *sys_info;
void *sysVMemPointer;
int sysVMemID; // system V memory segment id

void shutdown_server() {
    printf("\nServer terminated\n");
    if (sysVMemPointer) {
        shmdt(sysVMemPointer);
        printf("System-v-pointer released\n");
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

    int msgQID;
    msgbuf_t msg;

    // server setup from args
    int opt = 0, run_mode = UNDEFMODE;
    while ((opt = getopt(argc, argv, "vmq")) != -1) {
        switch (opt) {
            case 'v':
                run_mode = SYSVMODE;
                // initialize system v memory segment and set system info pointer to
                // system v pointer
                sysVMemID = shmget(IPC_PRIVATE, sizeof(struct system_info), IPC_CREAT | 0644);
                sysVMemPointer = shmat(sysVMemID, NULL, 0);
                sys_info = (struct system_info *) sysVMemPointer;
                break;
            case 'm':
                run_mode = MMAPMODE;
                // create file and map it to memory
                sprintf(filename, "%d", pid);
                mmapFD = open(filename, O_RDWR | O_CREAT, 0644);  // create file
                ftruncate(mmapFD, sizeof(struct system_info));    // set file length
                sys_info = (struct system_info *) mmap(NULL, sizeof(struct system_info),
                                                       PROT_WRITE | PROT_READ, MAP_SHARED, mmapFD,
                                                       0); // map file to memory
                break;
            case 'q':
                run_mode = MSGQMODE;
                // initialize system_info structure
                sys_info = (struct system_info *) malloc(sizeof(struct system_info));
                // initialize message queue
                msgQID = msgget(IPC_PRIVATE, IPC_CREAT | 0644);
                break;
        }
    }
    if (run_mode == UNDEFMODE) {
        fprintf(stderr, "Usage: %s [-v] [-m] [-q]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("Init parameters:\n");
    printf("pid = %u\n", pid);
    printf("uid = %u\n", uid);
    printf("gid = %u\n\n", gid);

    switch (run_mode) {
        case SYSVMODE:
            printf("system-v-shared-memory mode\n");
            printf("Created system-v-memory-segment\n");
            printf("id = %u\n", sysVMemID);
            printf("size = %lu bytes\n\n", sizeof(struct system_info));
            break;
        case MMAPMODE:
            printf("mmap mode\n");
            printf("Created new file:\n");
            printf("name = %s\n", filename);
            printf("size = %lu bytes\n\n", sizeof(struct system_info));
            break;
        case MSGQMODE:
            printf("system-v-message-queue mode\n");
            printf("Created new queue\n");
            printf("id = %u\n\n", msgQID);
            break;
    }


    // initialize sys_info struct
    sys_info->pid = pid;
    sys_info->uid = uid;
    sys_info->gid = gid;
    sys_info->startup_time = time(NULL) - start_time;
    getloadavg(sys_info->sys_loads, 3);

    printf("Server is running...\n");
    while (1) {
        if (run_mode == MSGQMODE) {
            // receive message
            msgrcv(msgQID, &msg, 0, MSGTYPE_QUERY, 0);
            // updating current time
            sys_info->startup_time = time(NULL) - start_time;
            // updating average system load
            getloadavg(sys_info->sys_loads, 3);
            // send reply
            msg.mtype = MSGTYPE_REPLY;
            memcpy(msg.mtext, sys_info, sizeof(struct system_info));
            msgsnd(msgQID, &msg, sizeof(struct system_info), 0);
        } else {
            sleep(1);
            // updating current time
            sys_info->startup_time = time(NULL) - start_time;
            // updating average system load
            getloadavg(sys_info->sys_loads, 3);
        }
    }
}