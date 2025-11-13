```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024

char *get_ls_result(const char *dir_path) {
    // Construct the command string
    char command[MAX_COMMAND_LENGTH];
    snprintf(command, sizeof(command), "ls -l %s", dir_path);

    // Prepare pipes for capturing the command output
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        return "";
    }

    // Fork a child process
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

        // Execute the command
        if (execl("/bin/sh", "sh", "-c", command, NULL) == -1) {
            _exit(1);
        }
    } else {
        // Parent process: read the output from the read end of the pipe
        close(pipefd[1]);

        char buffer[1024];
        ssize_t bytes_read;
        size_t total_bytes = 0;
        char *result = malloc(MAX_COMMAND_LENGTH * sizeof(char));
        result[0] = '\0';

        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            total_bytes += bytes_read;
            if (total_bytes >= MAX_COMMAND_LENGTH) {
                free(result);
                result = realloc(result, (MAX_COMMAND_LENGTH * 2) * sizeof(char));
                total_bytes = MAX_COMMAND_LENGTH;
            }
            strncat(result, buffer, bytes_read);
        }

        close(pipefd[0]);

        // Wait for the child process to finish
        waitpid(pid, NULL, 0);

        return result;
    }

    // This point should never be reached
    return "";
}
```