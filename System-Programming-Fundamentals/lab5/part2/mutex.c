#include "alphabet.h"
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void lock() {
    pthread_mutex_lock(&mutex);
}

void unlock() {
    pthread_mutex_unlock(&mutex);
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
    int inv_interval = 100, swp_interval = 200, main_interval = 1000;
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
            default:
                fprintf(stderr, "Usage: %s [-i] [-s] [-m]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    pthread_mutex_init(&mutex, NULL);

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