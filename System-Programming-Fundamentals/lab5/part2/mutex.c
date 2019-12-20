#include "alphabet.h"
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int invert_interval = 100, change_case_interval = 200, main_interval = 1000;

void lock() {
    pthread_mutex_lock(&mutex);
}

void unlock() {
    pthread_mutex_unlock(&mutex);
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

int main(int argc, char *argv[]) {
    int opt = 0;
    while ((opt = getopt(argc, argv, "i:c:m:")) != -1) {
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
            default:
                fprintf(stderr, "Usage: %s [-i] [-c] [-m]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    pthread_mutex_init(&mutex, NULL);

    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, invert_alphabet, NULL);
    pthread_create(&thread2, NULL, change_case, NULL);

    while (1) {
        usleep(main_interval);
        lock();
        print_alphabet();
        unlock();
    }
}