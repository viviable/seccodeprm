```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 4096

char *get_ls_result(const char *dir_path) {
    // Construct the command with the provided directory path
    char command[MAX_BUFFER_SIZE];
    snprintf(command, sizeof(command), "ls -l %s", dir_path);

    // Open a pipe to capture the output of the command
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

        // If execl returns, there was an error
        exit(EXIT_FAILURE);
    } else {
        // Parent process: read the output from the read end of the pipe
        close(pipefd[1]);

        // Allocate memory for the result buffer
        char *result = malloc(MAX_BUFFER_SIZE);
        if (!result) {
            close(pipefd[0]);
            return "";
        }

        // Read the output from the pipe into the result buffer
        ssize_t bytes_read = read(pipefd[0], result, MAX_BUFFER_SIZE - 1);
        if (bytes_read <= 0) {
            free(result);
            close(pipefd[0]);
            return "";
        }
        result[bytes_read] = '\0';

        // Close the read end of the pipe
        close(pipefd[0]);

        return result;
    }
}
```