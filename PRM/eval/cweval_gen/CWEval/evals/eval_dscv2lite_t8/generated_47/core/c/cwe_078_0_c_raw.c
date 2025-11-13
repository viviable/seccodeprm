```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_OUTPUT_SIZE 1024

char *get_ls_result(const char *dir_path) {
    // Define the command with the provided directory path
    char command[MAX_OUTPUT_SIZE];
    snprintf(command, sizeof(command), "ls -l %s", dir_path);

    // Open a pipe to capture the output of the command
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        return ""; // Return empty string on error
    }

    // Fork a new process to execute the command
    pid_t pid = fork();
    if (pid == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        return ""; // Return empty string on error
    } else if (pid == 0) {
        // Child process: redirect stdout to the write end of the pipe
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        // Execute the command
        execl("/bin/sh", "sh", "-c", command, NULL);

        // If execl returns, it means there was an error
        _exit(EXIT_FAILURE);
    } else {
        // Parent process: read the output from the read end of the pipe
        close(pipefd[1]);

        char output[MAX_OUTPUT_SIZE];
        ssize_t bytes_read = read(pipefd[0], output, sizeof(output) - 1);
        if (bytes_read > 0) {
            output[bytes_read] = '\0'; // Null-terminate the string
        }
        close(pipefd[0]);

        // Wait for the child process to finish
        wait(NULL);

        return strdup(output); // Duplicate the output string to return
    }
}
```