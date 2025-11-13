```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_OUTPUT_SIZE 4096

char *get_ls_result(const char *dir_path) {
    char *output = malloc(MAX_OUTPUT_SIZE);
    if (!output) {
        return "";
    }

    // Construct the command with the provided directory path
    char command[MAX_OUTPUT_SIZE];
    snprintf(command, sizeof(command), "ls -l %s", dir_path);

    // Open a pipe to capture the command output
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        free(output);
        return "";
    }

    pid_t pid = fork();
    if (pid == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        free(output);
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
        // Parent process: read the output from the read end of the pipe
        close(pipefd[1]);

        ssize_t bytes_read;
        while ((bytes_read = read(pipefd[0], output + strlen(output), MAX_OUTPUT_SIZE - strlen(output) - 1)) > 0) {
            output[strlen(output)] = '\0';
        }

        close(pipefd[0]);

        // Wait for the child process to finish
        waitpid(pid, NULL, 0);
    }

    return output;
}
```