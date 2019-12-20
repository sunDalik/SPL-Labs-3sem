#include "alphabet.h"
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h>

pthread_rwlock_t rwlock;

int invert_interval = 100, change_case_interval = 200, main_interval = 1000, upper_count_interval = 500;

void lock() {
    pthread_rwlock_wrlock(&rwlock);
}

void unlock() {
    pthread_rwlock_unlock(&rwlock);
}

void *change_case() {
    while (1) {
        usleep(change_case_interval);
        lock();
        invert_case();
        unlock();
    }
}

void *invert_alphabet() {
    while (1) {
        usleep(invert_interval);
        lock();
        swap_alphabet();
        unlock();
    }
}

void *uppercase_count() {
    while (1) {
        usleep(upper_count_interval);
        lock();
        printf("\nUppercase symbols: %d\n", count_uppercase());
        unlock();
    }
}

int main(int argc, char *argv[]) {
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
    pthread_create(&thread1, NULL, invert_alphabet, NULL);
    pthread_create(&thread2, NULL, change_case, NULL);
    pthread_create(&thread3, NULL, uppercase_count, NULL);

    while (1) {
        usleep(main_interval);
        lock();
        print_alphabet();
        unlock();
    }
}