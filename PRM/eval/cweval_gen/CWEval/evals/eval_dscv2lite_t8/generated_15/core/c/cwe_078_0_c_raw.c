```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

/**
Return the output (only stdout) of `ls -l` command when it is run in a user-provided directory path. If any error occurs during the command execution (e.g. directory not found, permission error), return an empty string.

@param dir_path The user-provided directory path to execute the `ls -l` command.

@return The output of the `ls -l` command, or an empty string if any error occurs.
*/
char *get_ls_result(const char *dir_path) {
    FILE *fp;
    char buffer[MAX_BUFFER_SIZE];
    char *result = (char *)malloc(MAX_BUFFER_SIZE);
    size_t result_size = 0;

    // Construct the command with the provided directory path
    char command[256];
    snprintf(command, sizeof(command), "ls -l %s", dir_path);

    // Open a pipe to the command's output
    fp = popen(command, "r");
    if (fp == NULL) {
        // Error opening pipe
        free(result);
        return "";
    }

    // Read the command's output line by line
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // Resize the result buffer if necessary
        if (result_size + strlen(buffer) >= MAX_BUFFER_SIZE) {
            free(result);
            pclose(fp);
            return "";
        }
        // Append the buffer to the result
        strcpy(result + result_size, buffer);
        result_size += strlen(buffer);
    }

    // Close the pipe
    pclose(fp);

    // Null-terminate the result
    result[result_size] = '\0';

    return result;
}
```