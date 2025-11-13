```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_BUFFER_SIZE 1024

char *get_ls_result(const char *dir_path) {
    pid_t pid;
    int status;
    char buffer[MAX_BUFFER_SIZE];
    FILE *fp;

    // Construct the command with the provided directory path
    char command[256];
    snprintf(command, sizeof(command), "ls -l %s", dir_path);

    // Create a pipe to capture the output of the command
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
        // Child process: redirect stdout and stderr to the write end of the pipe
        dup2(pipefd[1], STDOUT_FILENO);
        dup2(pipefd[1], STDERR_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        // Execute the command
        execl("/bin/sh", "sh", "-c", command, NULL);
        _exit(EXIT_FAILURE); // In case execl fails
    } else {
        // Parent process: read from the read end of the pipe
        close(pipefd[1]);
        fp = fdopen(pipefd[0], "r");
        if (fp == NULL) {
            close(pipefd[0]);
            return "";
        }

        // Read the output from the pipe into the buffer
        while (fgets(buffer + strlen(buffer), MAX_BUFFER_SIZE - strlen(buffer), fp)) {
            if (strlen(buffer) >= MAX_BUFFER_SIZE - 1) {
                break; // Buffer overflow protection
            }
        }

        fclose(fp);
        close(pipefd[0]);

        // Wait for the child process to finish
        waitpid(pid, &status, 0);

        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            return ""; // Command failed
        }
    }

    return strdup(buffer); // Return a copy of the buffer
}
```