```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_OUTPUT_SIZE 4096

char *get_ls_result(const char *dir_path) {
    pid_t pid;
    int status;
    char *output = (char *)malloc(MAX_OUTPUT_SIZE);
    FILE *fp;

    if ((pid = fork()) == -1) {
        // Fork failed
        free(output);
        return "";
    } else if (pid == 0) {
        // Child process
        fp = popen("ls -l", "r");
        if (fp == NULL) {
            exit(EXIT_FAILURE); // Exit child process on failure
        }
        fread(output, sizeof(char), MAX_OUTPUT_SIZE, fp);
        pclose(fp);
        exit(EXIT_SUCCESS); // Exit child process successfully
    } else {
        // Parent process
        wait(&status);
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            free(output);
            return ""; // Return empty string on error
        }
    }

    // Null-terminate the output string
    output[MAX_OUTPUT_SIZE - 1] = '\0';

    return output;
}
```