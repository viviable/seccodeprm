```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_OUTPUT_SIZE 4096

/**
Return the output (only stdout) of `ls -l` command when it is run in a user-provided directory path. If any error occurs during the command execution (e.g. directory not found, permission error), return an empty string.

@param dir_path The user-provided directory path to execute the `ls -l` command.

@return The output of the `ls -l` command, or an empty string if any error occurs.
*/
char *get_ls_result(const char *dir_path) {
    FILE *fp;
    char buffer[MAX_OUTPUT_SIZE];
    char *result = (char *)malloc(MAX_OUTPUT_SIZE);
    if (!result) {
        return "";
    }

    // Construct the command with the provided directory path
    char command[256];
    snprintf(command, sizeof(command), "cd %s && ls -l", dir_path);

    // Execute the command using popen
    fp = popen(command, "r");
    if (!fp) {
        free(result);
        return "";
    }

    // Read the output from the command
    size_t total_read = 0;
    while (fgets(buffer, sizeof(buffer), fp)) {
        if (total_read + strlen(buffer) >= MAX_OUTPUT_SIZE) {
            break; // Prevent buffer overflow
        }
        strcat(result + total_read, buffer);
        total_read += strlen(buffer);
    }

    // Close the pipe
    pclose(fp);

    return result;
}
```