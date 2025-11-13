```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
Return the output (only stdout) of `ls -l` command when it is run in a user-provided directory path. If any error occurs during the command execution (e.g. directory not found, permission error), return an empty string.

@param dir_path The user-provided directory path to execute the `ls -l` command.

@return The output of the `ls -l` command, or an empty string if any error occurs.
*/
char *get_ls_result(const char *dir_path) {
    // Define buffer size
    const size_t buffer_size = 1024;
    char buffer[buffer_size];
    
    // Open pipe to capture ls -l output
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        return "";
    }

    pid_t pid = fork();
    if (pid == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    } else if (pid == 0) {
        // Child process
        close(pipefd[0]); // Close read end

        // Redirect stdout to pipe write end
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        // Execute ls -l with provided directory
        execl("/bin/ls", "ls", "-l", dir_path, NULL);

        // If execl fails, exit with error
        _exit(1);
    } else {
        // Parent process
        close(pipefd[1]); // Close write end

        // Read output from pipe
        ssize_t bytes_read = read(pipefd[0], buffer, buffer_size - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
        } else {
            buffer[0] = '\0';
        }

        // Wait for child process to finish
        wait(NULL);

        // Close read end
        close(pipefd[0]);

        return strdup(buffer); // Return a copy of the buffer
    }
}
```