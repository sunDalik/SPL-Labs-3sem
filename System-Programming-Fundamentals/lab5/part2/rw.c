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

void *uppercase_count(void *args) {
    int *interval = (int *) args;
    for (;;) {
        usleep(*interval);
        pthread_rwlock_rdlock(&rwlock);
        int upper = count_uppercase();
        printf("\nUppercase symbols: %d\n", upper);
        pthread_rwlock_unlock(&rwlock);
    }
}

int main(int argc, char *argv[]) {
    int invert_interval = 100, change_case_interval = 200, main_interval = 1000, upper_count_interval = 10;
    int opt = 0;
    while ((opt = getopt(argc, argv, "i:c:m:u:")) != -1) {
        switch (opt) {
            case 'i':
                invert_interval = (int) strtol(optarg, NULL, 10);
                break;
            case 'c':
                change_case_interval = (int) strtol(optarg, NULL, 10);
                break;
            case 'm':
                main_interval = (int) strtol(optarg, NULL, 10);
                break;
            case 'u':
                upper_count_interval = (int) strtol(optarg, NULL, 10);
                break;
            default:
                fprintf(stderr, "Usage: %s [-i] [-c] [-m] [-u]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    pthread_rwlock_init(&rwlock, NULL);

    pthread_t thread1, thread2, thread3;
    pthread_create(&thread1, NULL, invert_alphabet, (void *) &invert_interval);
    pthread_create(&thread2, NULL, change_case, (void *) &change_case_interval);
    pthread_create(&thread3, NULL, uppercase_count, (void *) &upper_count_interval);

    while (1) {
        usleep(main_interval);
        lock();
        print_alphabet();
        unlock();
    }
}