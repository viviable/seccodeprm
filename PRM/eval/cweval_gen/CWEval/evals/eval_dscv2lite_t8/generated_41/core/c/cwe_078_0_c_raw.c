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
    // Temporary buffer size for storing the command output
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    FILE *fp;

    // Construct the command with the provided directory path
    snprintf(buffer, BUFFER_SIZE, "ls -l %s", dir_path);

    // Open a pipe to execute the command
    fp = popen(buffer, "r");
    if (fp == NULL) {
        // Return an empty string if there was an error opening the pipe
        return "";
    }

    // Read the output from the pipe into a dynamically allocated buffer
    char *result = (char *)malloc(BUFFER_SIZE);
    if (result == NULL) {
        // Close the pipe if malloc fails
        pclose(fp);
        return "";
    }
    result[0] = '\0';
    while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        result = realloc(result, strlen(result) + strlen(buffer) + 1);
        if (result == NULL) {
            // Close the pipe if realloc fails
            pclose(fp);
            return "";
        }
        strcat(result, buffer);
    }

    // Close the pipe
    pclose(fp);

    // Remove trailing newline characters
    size_t len = strlen(result);
    while (len > 0 && (result[len - 1] == '\n' || result[len - 1] == '\r')) {
        result[--len] = '\0';
    }

    return result;
}
```