#include "alphabet.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <semaphore.h>

sem_t sem;

void *change_case() {
    while (1) {
        sem_wait(&sem);

        invert_case();
        print_alphabet();

        sem_post(&sem);
        sleep(1);
    }
}

void *invert_alphabet() {
    while (1) {
        sem_wait(&sem);

        swap_alphabet();
        print_alphabet();

        sem_post(&sem);
        sleep(1);
    }
}

int main() {
    sem_init(&sem, 0, 1);
    
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, change_case, NULL);
    pthread_create(&thread2, NULL, invert_alphabet, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
}