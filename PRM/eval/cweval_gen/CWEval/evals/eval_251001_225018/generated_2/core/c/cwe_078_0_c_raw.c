```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 256

char *get_ls_result(const char *dir_path) {
    // Construct the full command with the provided directory path
    char command[MAX_COMMAND_LENGTH];
    snprintf(command, sizeof(command), "ls -l %s", dir_path);

    // Allocate memory for the output buffer
    char *output = (char *)malloc(1024); // Initial allocation size
    if (!output) {
        return "";
    }

    // Set up pipes for reading the command output
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
        _exit(EXIT_FAILURE); // Exit on failure
    } else {
        // Parent process: read from the read end of the pipe
        close(pipefd[1]);

        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            ssize_t bytes_read;
            while ((bytes_read = read(pipefd[0], output + strlen(output), 1023)) > 0) {
                output[strlen(output)] = '\0';
            }
            output[strcspn(output, "\n")] = '\0'; // Remove trailing newline
        } else {
            free(output);
            output = (char *)"";
        }

        close(pipefd[0]);
    }

    return output;
}
```