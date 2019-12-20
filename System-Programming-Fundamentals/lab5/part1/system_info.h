#ifndef LAB5_SYSTEM_INFO_H
#define LAB5_SYSTEM_INFO_H

#include <sys/types.h>

struct system_info {
    pid_t pid;
    uid_t uid;
    gid_t gid;
    time_t run_time;
    double sys_loads[3];
};

#define MSGTYPE_QUERY 1
#define MSGTYPE_REPLY 2
typedef struct {
    long mtype;
    char mtext[sizeof(struct system_info)];
} msg_t;

#endif
