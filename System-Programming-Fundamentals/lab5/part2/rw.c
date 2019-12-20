#include "alphabet.h"
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h>

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

void lock() {
    pthread_rwlock_wrlock(&rwlock);
}

void unlock() {
    pthread_rwlock_unlock(&rwlock);
}

void *change_case(void *args) {
    int *interval = (int *) args;
    while (1) {
        usleep(*interval);
        lock();
        invert_case();
        unlock();
    }
}

void *invert_alphabet(void *args) {
    int *interval = (int *) args;
    while (1) {
        usleep(*interval);
        lock();
        swap_alphabet();
        unlock();
    }
}

void *cnt_f(void *args) {
    int *interval = (int *) args;
    for (;;) {
        usleep(*interval);
        pthread_rwlock_rdlock(&rwlock);
        int upper = count_uppercase();
        printf("\nUppercase symbols: %d\n", upper);
        pthread_rwlock_unlock(&rwlock);
    }
}

bool parse_int(const char *str, int *var) {
    char *end;
    *var = (int) strtol(str, &end, 10);
    if (end == str) return false;
    return true;
}

int main(int argc, char *argv[]) {
    // thread descriptors init
    pthread_t inv_thread, swp_thread;
    // setting up intervals
    int inv_interval = 100, swp_interval = 200, main_interval = 1000, cnt_interval = 10;
    int opt = 0;
    while ((opt = getopt(argc, argv, "i:s:m:c:")) != -1) {
        switch (opt) {
            case 'i':
                if (!parse_int(optarg, &inv_interval)) {
                    printf("-i parameter must be integer!");
                    return 1;
                }
                break;
            case 's':
                if (!parse_int(optarg, &swp_interval)) {
                    printf("-s parameter must be integer!");
                    return 1;
                }
                break;
            case 'm':
                if (!parse_int(optarg, &main_interval)) {
                    printf("-m parameter must be integer!");
                    return 1;
                }
                break;
            case 'c':
                if (!parse_int(optarg, &cnt_interval)) {
                    printf("-c parameter must be integer!");
                    return 1;
                }
                break;
            default:
                fprintf(stderr, "Usage: %s [-i] [-s] [-m] [-c]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    // initialize rwlock
    pthread_rwlock_init(&rwlock, NULL);

    pthread_t cnt_thread;
    pthread_create(&cnt_thread, NULL, cnt_f, (void *) &cnt_interval);

    // THREADS INIT
    pthread_create(&inv_thread, NULL, change_case, (void *) &inv_interval);
    pthread_create(&swp_thread, NULL, invert_alphabet, (void *) &swp_interval);

    while (1) {
        usleep(main_interval);
        lock();
        print_alphabet();
        unlock();
    }
}