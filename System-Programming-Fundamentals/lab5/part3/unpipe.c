#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <zconf.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s file\n", argv[0]);
        return EXIT_FAILURE;
    }

    int fd = open(argv[1], O_RDONLY);
    int pipe_fd[2];
    pipe(pipe_fd);
    int pipe_fd_rd = pipe_fd[0];
    int pipe_fd_wr = pipe_fd[1];

    int wc_pid = fork();
    if (wc_pid == 0) {
        //child process
        close(pipe_fd_wr);
        dup2(pipe_fd_rd, STDIN_FILENO);
        execlp("wc", "wc", NULL);
        close(pipe_fd_rd);
    } else {
        //parent process
        close(pipe_fd_rd);
        char file_char;
        while (read(fd, &file_char, 1) > 0) {
            write(pipe_fd_wr, &file_char, 1);
            read(fd, &file_char, 1); //skip one symbol
        }
        close(pipe_fd_wr);
    }
    close(fd);
    return EXIT_SUCCESS;
}