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
#include <signal.h>
#include "message_format.h"

#define PER_THREAD 1
#define PER_TASK 2
#define THREAD_POOL 3

int metrics_report_interval = 100; //milliseconds
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
        ssize_t curr_read = read(fd, (char *) buf + bytes_read, bytes - bytes_read);
        //EOF
        if (curr_read <= 0)
            return false;
        bytes_read += curr_read;
    }
    return true;
}

void goodbye() {
    printf("\nGoodbye!\n");
    pthread_cancel(reader_thread);
    pthread_cancel(writer_thread);
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    close(fileno(result_log_file));
    close(fileno(metrics_log_file));
    exit(EXIT_SUCCESS);
}

void lock() {
    pthread_mutex_lock(&mutex);
}

void unlock() {
    pthread_mutex_unlock(&mutex);
}

uint32_t fibonacci(uint32_t n) {
    if (n <= 1)
        return 1;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

void bubble_sort(uint32_t arr[], int n) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                uint32_t temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void write_tmessage(int fd, TMessage message) {
    write(fd, &message.Type, sizeof(message.Type));
    write(fd, &message.Size, sizeof(message.Size));
    write(fd, message.Data, message.Size);
}

TMessage read_to_tmessage(int fd) {
    TMessage message;
    if (read_all(fd, &message.Type, sizeof(message.Type)) == false) return empty_message;
    if (read_all(fd, &message.Size, sizeof(message.Size)) == false) return empty_message;
    message.Data = calloc(message.Size, 1);
    if (read_all(fd, message.Data, message.Size) == false) return empty_message;
    return message;
}

TMessage read_tmessage(int fd) {
    TMessage message;
    if (read_all(fd, &message.Type, sizeof(message.Type)) == false) return empty_message;
    if (read_all(fd, &message.Size, sizeof(message.Size)) == false) return empty_message;
    if (read_all(fd, &message.Data, sizeof(message.Data)) == false) return empty_message;
    return message;
}

void *writer() {
    while (1) {
        TMessage msg = read_tmessage(pipe_fd[0]);
        if (msg.Type != NONE) {
            printf("WRITER writing: %d, type: %d\n", *msg.Data, msg.Type);
            write_tmessage(fileno(result_log_file), msg);
        }
    }
}

void *reader() {
    while (1) {
        TMessage msg = read_to_tmessage(STDIN_FILENO);
        TMessage out_msg;
        out_msg.Type = msg.Type;
        uint32_t result;
        switch (msg.Type) {
            case FIBONACCI:
                result = fibonacci(*msg.Data);
                out_msg.Size = 4;
                out_msg.Data = &result;
                break;
            case POW:
                result = pow(msg.Data[0], msg.Data[1]);
                out_msg.Size = 4;
                out_msg.Data = &result;
                break;
            case BUBBLE_SORT_UINT64:
                bubble_sort(msg.Data, msg.Size / 4);
                out_msg.Size = msg.Size;
                out_msg.Data = msg.Data;
                break;
            case NONE:
                return 0;
        }
        printf("READER send to writer thread: %d, type: %d\n", *out_msg.Data, out_msg.Type);
        write(pipe_fd[1], &out_msg, sizeof(TMessage));

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
    empty_message.Type = NONE;
    struct sigaction action = {.sa_handler = goodbye, .sa_flags = 0};
    sigaction(SIGHUP, &action, NULL);
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);
    sigaction(SIGKILL, &action, NULL);

    static struct option long_options[] = {{"thread_count", required_argument, 0, 't'},
                                           {"strategy",     required_argument, 0, 's'},
                                           {NULL, 0, NULL,                        0}};
    int opt = 0;
    while ((opt = getopt_long(argc, argv, "t:s:n:", long_options, NULL)) != -1) {
        switch (opt) {
            case 'n':
                metrics_report_interval = (int) strtol(optarg, NULL, 10);
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

