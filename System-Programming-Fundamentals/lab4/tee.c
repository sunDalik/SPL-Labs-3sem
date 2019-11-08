#include <errno.h>
#include <getopt.h>
#include <string.h>
#include <malloc.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

extern int errno;

const char *INVALID_OPTION_MESSAGE = "Invalid option\n";
const char *USAGE_MESSAGE = "Usage: tee [-ai] [FILE]...\n";

int append_mode = 0;
int ignore_interrupts_mode = 0;
int errors = 0;

void handle_interruptions() {
    if (!ignore_interrupts_mode) {
        write(2, "\n", 1);
        exit(errors);
    }
}

/* tee */
int main(int argc, char *argv[]) {
    signal(SIGINT, handle_interruptions);
    int rez = 0;
    while ((rez = getopt(argc, argv, "ai")) != -1) {
        switch (rez) {
            case 'a':
                append_mode = 1;
                break;
            case 'i':
                ignore_interrupts_mode = 1;
                break;
            case '?':
                write(2, INVALID_OPTION_MESSAGE, strlen(INVALID_OPTION_MESSAGE));
                write(2, USAGE_MESSAGE, strlen(USAGE_MESSAGE));
                return 1;
        }
    }

    const int FILE_COUNT = argc - optind;
    FILE **files = malloc(FILE_COUNT * sizeof(int));
    int f_i = 0;
    for (int i = optind; i < argc; ++i) {
        if (append_mode) {
            files[f_i] = fopen(argv[i], "a");
        } else {
            files[f_i] = fopen(argv[i], "w");
        }

        if (files[f_i] == NULL) {
            errors = 1;
            if (errno == EACCES) {
                fprintf(stderr, "Permission denied: %s\n", argv[i]);
            }
        }

        f_i++;
    }


    char input;
    while (read(0, &input, 1) > 0) {
        write(1, &input, 1);
        for (int i = 0; i < FILE_COUNT; ++i) {
            if (files[i] != NULL) {
                fputc(input, files[i]);
            }
        }
    }

    for (int i = 1; i < FILE_COUNT; ++i) {
        if (files[i] != NULL) fclose(files[i]);
    }
    free(files);
    return errors;
}
