#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <getopt.h>

#define PER_THREAD 1;
#define PER_TASK 2;
#define THREAD_POOL 3;

int main(int argc, char *argv[]) {
    int report_interval = 100; //milliseconds
    int thread_count = 3;
    int strategy = PER_THREAD;

    static struct option long_options[] = {{"thread_count", required_argument, 0, 't'},
                                           {"strategy",     required_argument, 0, 's'},
                                           {NULL, 0, NULL,                        0}};
    int opt = 0;
    while ((opt = getopt_long(argc, argv, "t:s:n:", long_options, NULL)) != -1) {
        switch (opt) {
            case 'n':
                report_interval = (int) strtol(optarg, NULL, 10);
                break;
            case 't':
                thread_count = (int) strtol(optarg, NULL, 10);
                break;
            case 's':
                if (strncmp(optarg, "per_thread", 11) == 0) {
                    strategy = PER_THREAD;
                } else if (strncmp(optarg, "per_task", 11) == 0) {
                    strategy = PER_TASK;
                } else if (strncmp(optarg, "thread_pool", 11) == 0) {
                    strategy = THREAD_POOL;
                }
                break;
        }
    }
}