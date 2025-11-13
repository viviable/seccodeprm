```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_OUTPUT_SIZE 1024

char *get_ls_result(const char *dir_path) {
    // Buffer to hold the output of the ls command
    char buffer[MAX_OUTPUT_SIZE];
    FILE *fp;
    pid_t pid;

    // Construct the command with the provided directory path
    char command[256];
    snprintf(command, sizeof(command), "ls -l %s", dir_path);

    // Open a pipe to capture the command output
    fp = popen(command, "r");
    if (fp == NULL) {
        // Return an empty string if popen fails
        return "";
    }

    // Read the output from the pipe into the buffer
    while (fgets(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), fp) != NULL) {
        // Ensure the buffer does not overflow
        if (strlen(buffer) >= MAX_OUTPUT_SIZE - 1) {
            break;
        }
    }

    // Close the pipe
    pclose(fp);

    // Allocate memory for the result string
    char *result = malloc(strlen(buffer) + 1);
    if (result == NULL) {
        // Return an empty string if memory allocation fails
        return "";
    }

    // Copy the buffer content to the result string
    strcpy(result, buffer);

    return result;
}
```