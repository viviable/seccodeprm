```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_BUFFER_SIZE 1024

char *get_ls_result(const char *dir_path) {
    char buffer[MAX_BUFFER_SIZE];
    pid_t pid;
    int status;

    // Construct the command with the provided directory path
    char command[256];
    snprintf(command, sizeof(command), "ls -l %s", dir_path);

    // Pipe to capture the output of the command
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        return "";
    }

    // Fork a child process to execute the command
    pid = fork();
    if (pid == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    } else if (pid == 0) {
        // Child process
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        // Execute the command
        execl("/bin/sh", "sh", "-c", command, NULL);

        // If execl returns, there was an error
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        close(pipefd[1]);

        // Read the output from the pipe
        ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
        } else {
            buffer[0] = '\0';
        }

        // Wait for the child process to finish
        waitpid(pid, &status, 0);

        // Close the pipe
        close(pipefd[0]);

        return strdup(buffer); // Return a copy of the buffer
    }
}
```