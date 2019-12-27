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
#include <pthread.h>
#include <math.h>
#include <fcntl.h>
#include "message_format.h"

#define PER_THREAD 1
#define PER_TASK 2
#define THREAD_POOL 3

int report_interval = 100; //milliseconds
int thread_count = 3;
int strategy = PER_THREAD;
pthread_mutex_t mutex;
pthread_t reader_thread, writer_thread;
int pipe_fd[2];
FILE *result_log_file;
FILE *metrics_log_file;
TMessage empty_message;

bool read_all(int fd, void *buf, size_t bytes) {
    size_t bytes_read = 0;
    while (bytes_read < bytes) {
        int curr_read = read(fd, buf + bytes_read, bytes - bytes_read);
        //EOF
        if (curr_read == 0)
            return false;
        bytes_read += curr_read;
    }
    return true;
}

void lock() {
    pthread_mutex_lock(&mutex);
}

void unlock() {
    pthread_mutex_unlock(&mutex);
}

uint64_t fibonacci(uint64_t n) {
    if (n <= 1)
        return 1;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

void write_tmessage(int fd, TMessage message) {
    write(fd, &message.Type, sizeof(uint8_t));
    write(fd, &message.Size, sizeof(uint64_t));
    write(fd, message.Data, message.Size);
}

TMessage read_tmessage(int fd) {
    TMessage message;
    if (read_all(fd, &message.Type, sizeof(uint8_t)) == false) return empty_message;
    if (read_all(fd, &message.Size, sizeof(uint64_t)) == false) return empty_message;
    message.Data = calloc(message.Size, 1);
    if (read_all(fd, message.Data, message.Size) == false) return empty_message;
    return message;
}

void *writer() {
    int i = 1;
    while (1) {
        //usleep(1000);
        TMessage msg;
        if (read_all(pipe_fd[0], &msg, sizeof(TMessage)) > 0) {
            write_tmessage(fileno(result_log_file), msg);
            printf("))))\n");
        }
        printf("after alooo %d\n", i++);
    }
}

void *reader() {
    while (1) {
        TMessage msg = read_tmessage(STDIN_FILENO);
        TMessage out_msg;
        uint64_t result;
        switch (msg.Type) {
            case FIBONACCI:
                result = fibonacci(*((uint64_t *) msg.Data));
                out_msg.Type = FIBONACCI;
                out_msg.Size = 8;
                out_msg.Data = (u_int8_t *) result;
                write(pipe_fd[1], &out_msg, sizeof(TMessage));
                break;
            case POW:
                result = pow(*((uint64_t *) msg.Data), *(((uint64_t *) msg.Data) + 1));
                out_msg.Type = POW;
                out_msg.Size = 8;
                out_msg.Data = (u_int8_t *) result;
                write(pipe_fd[1], &out_msg, sizeof(TMessage));
                break;
            case BUBBLE_SORT_UINT64:

                break;
            case STOP:

                break;
        }


        switch (strategy) {
            case PER_THREAD:
                //create new thread for each task

                //pthread_create(&hz, NULL, reader, NULL);
                break;
            case PER_TASK:
                //
                break;
            case THREAD_POOL:
                //
                break;
        }
    }
}

int main(int argc, char *argv[]) {
    empty_message.Type = -1;

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
                if (strncmp(optarg, "per_thread", 10) == 0) {
                    strategy = PER_THREAD;
                } else if (strncmp(optarg, "per_task", 8) == 0) {
                    strategy = PER_TASK;
                } else if (strncmp(optarg, "thread_pool", 11) == 0) {
                    strategy = THREAD_POOL;
                }
                break;
        }
    }

    pthread_mutex_init(&mutex, NULL);

    pipe(pipe_fd);

    result_log_file = fopen("result_log", "w");
    metrics_log_file = fopen("metrics_log", "w");

    pthread_create(&reader_thread, NULL, reader, NULL);
    pthread_create(&writer_thread, NULL, writer, NULL);
    pthread_join(reader_thread, NULL);
    pthread_join(writer_thread, NULL);
    return EXIT_SUCCESS;
}