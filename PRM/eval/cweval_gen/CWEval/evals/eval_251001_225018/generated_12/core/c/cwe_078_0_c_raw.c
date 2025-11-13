```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#define MAX_COMMAND_SIZE 256

char *get_ls_result(const char *dir_path) {
    // Construct the full command with the provided directory path
    char command[MAX_COMMAND_SIZE];
    snprintf(command, sizeof(command), "ls -l %s", dir_path);

    // Create a pipe to capture the output of the command
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        return "";
    }

    // Fork a child process to execute the command
    pid_t pid = fork();
    if (pid == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    } else if (pid == 0) {
        // Child process: redirect stdout to the write end of the pipe
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        // Execute the command
        execl("/bin/sh", "sh", "-c", command, NULL);
        _exit(1); // Exit if execl fails
    } else {
        // Parent process: read from the read end of the pipe
        close(pipefd[1]);

        // Allocate memory for the output buffer
        const size_t buffer_size = 4096;
        char *output = malloc(buffer_size);
        if (!output) {
            close(pipefd[0]);
            return "";
        }

        ssize_t bytes_read = read(pipefd[0], output, buffer_size - 1);
        close(pipefd[0]);

        if (bytes_read == -1) {
            free(output);
            return "";
        }

        output[bytes_read] = '\0';

        // Wait for the child process to finish
        wait(NULL);

        return output;
    }
}
```