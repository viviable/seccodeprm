```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 4096

char *get_ls_result(const char *dir_path) {
    char buffer[MAX_BUFFER_SIZE];
    pid_t pid;
    int pipefd[2];

    // Create a pipe to capture the output of the ls command
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return "";
    }

    // Fork a child process to run the ls command
    pid = fork();
    if (pid == -1) {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    } else if (pid == 0) {
        // Child process
        close(pipefd[0]); // Close read end of the pipe

        // Redirect stdout to the write end of the pipe
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        // Execute the ls command with the provided directory path
        execlp("ls", "ls", "-l", dir_path, NULL);

        // If execlp returns, there was an error
        perror("execlp");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        close(pipefd[1]); // Close write end of the pipe

        // Read the output from the pipe
        ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
        } else {
            buffer[0] = '\0';
        }

        close(pipefd[0]);

        // Wait for the child process to finish
        waitpid(pid, NULL, 0);

        return strdup(buffer); // Return a copy of the buffer
    }
}
```