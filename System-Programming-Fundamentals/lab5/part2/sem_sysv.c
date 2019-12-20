#include "alphabet.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
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

void *change_case() {
    while (1) {
        lock();
        invert_case();
        print_alphabet();
        unlock();
        sleep(1);
    }
}

void *invert_alphabet() {
    while (1) {
        lock();
        swap_alphabet();
        print_alphabet();
        unlock();
        sleep(1);
    }
}

int main() {
    sem_id = semget(IPC_PRIVATE, 2, IPC_CREAT | 0600);
    semctl(sem_id, 0, SETALL, 1);

    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, change_case, NULL);
    pthread_create(&thread2, NULL, invert_alphabet, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
}