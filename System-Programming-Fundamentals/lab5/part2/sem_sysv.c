#include "alphabet.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int sem_id;

void lock() {
    struct sembuf operations[2] = {
            {.sem_op = 0},
            {.sem_op = 1}
    };
    semop(sem_id, operations, 2);
}

void unlock() {
    struct sembuf operations = {.sem_op = -1};
    semop(sem_id, &operations, 1);
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

int main() {
    // thread descriptors init
    pthread_t inv_thread, swp_thread;

    sem_id = semget(IPC_PRIVATE, 2, 0600 | IPC_CREAT);
    semctl(sem_id, 0, SETALL, 1);

    // THREADS INIT
    pthread_create(&inv_thread, NULL, change_case, NULL);
    pthread_create(&swp_thread, NULL, invert_alphabet, NULL);

    while (1) {
        usleep(1000);
        lock();
        print_alphabet();
        unlock();
    }
}