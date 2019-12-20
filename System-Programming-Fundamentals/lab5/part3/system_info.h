#ifndef LAB5_SYSTEM_INFO_H
#define LAB5_SYSTEM_INFO_H

#include <sys/types.h>

struct system_info {
    pid_t pid;
    uid_t uid;
    gid_t gid;
    time_t startup_time;
    double sys_loads[3];
};
#endif
