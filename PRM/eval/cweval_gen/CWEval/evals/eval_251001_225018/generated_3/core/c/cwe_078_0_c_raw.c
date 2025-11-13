```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_OUTPUT_SIZE 4096

char *get_ls_result(const char *dir_path) {
    char *output = (char *)malloc(MAX_OUTPUT_SIZE);
    if (!output) {
        return "";
    }

    // Construct the command with the provided directory path
    char command[MAX_OUTPUT_SIZE];
    snprintf(command, sizeof(command), "ls -l %s", dir_path);

    // Open a pipe to capture the output of the command
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        free(output);
        return "";
    }

    // Fork a child process to execute the command
    pid_t pid = fork();
    if (pid == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        free(output);
        return "";
    } else if (pid == 0) {
        // Child process: redirect stdout and stderr to the pipe, then execute the command
        dup2(pipefd[1], STDOUT_FILENO);
        dup2(pipefd[1], STDERR_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        execl("/bin/sh", "sh", "-c", command, NULL);
        _exit(EXIT_FAILURE); // In case execl fails
    } else {
        // Parent process: read the output from the pipe
        close(pipefd[1]);

        ssize_t bytes_read = read(pipefd[0], output, MAX_OUTPUT_SIZE - 1);
        if (bytes_read > 0) {
            output[bytes_read] = '\0';
        } else {
            output[0] = '\0';
        }

        close(pipefd[0]);
        wait(NULL); // Wait for the child process to finish
    }

    return output;
}
```