```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024

char *get_ls_result(const char *dir_path) {
    // Construct the command with the provided directory path
    char command[MAX_COMMAND_LENGTH];
    snprintf(command, sizeof(command), "ls -l %s", dir_path);

    // Create a pipe to capture the output of the command
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
            _exit(EXIT_FAILURE);
        }
    } else {
        // Parent process: read the output from the pipe
        close(pipefd[1]);

        char buffer[1024];
        ssize_t bytes_read;
        char *output = (char *)malloc(MAX_COMMAND_LENGTH * sizeof(char));
        if (output == NULL) {
            close(pipefd[0]);
            wait(NULL);
            return "";
        }

        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            strncat(output, buffer, bytes_read);
        }

        close(pipefd[0]);
        wait(NULL);

        // Null-terminate the output string
        output[strcspn(output, "\n")] = '\0';

        return output;
    }
}
```