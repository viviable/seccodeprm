```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_COMMAND_LENGTH 1024

char *get_ls_result(const char *dir_path) {
    // Construct the full command with the provided directory path
    char command[MAX_COMMAND_LENGTH];
    snprintf(command, sizeof(command), "ls -l %s", dir_path);

    // Open a pipe to capture the command output
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
        // Child process: redirect stdout to the write end of the pipe and execute the command
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        if (execl("/bin/sh", "sh", "-c", command, NULL) == -1) {
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process: read the output from the read end of the pipe
        close(pipefd[1]);

        char buffer[1024];
        ssize_t bytes_read;
        size_t total_bytes = 0;
        char *result = (char *)malloc(MAX_COMMAND_LENGTH * sizeof(char));
        if (!result) {
            close(pipefd[0]);
            return "";
        }

        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            if (total_bytes + bytes_read >= MAX_COMMAND_LENGTH) {
                free(result);
                result = (char *)realloc(result, (MAX_COMMAND_LENGTH + 1024) * sizeof(char));
                if (!result) {
                    close(pipefd[0]);
                    return "";
                }
                MAX_COMMAND_LENGTH += 1024;
            }
            memcpy(result + total_bytes, buffer, bytes_read);
            total_bytes += bytes_read;
        }

        close(pipefd[0]);
        wait(NULL); // Wait for the child process to finish

        result[total_bytes] = '\0'; // Null-terminate the result string
        return result;
    }
}
```