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
    // Buffer size for the output
    const int buffer_size = 1024;
    char buffer[buffer_size];
    FILE *fp;

    // Construct the command with the provided directory path
    snprintf(buffer, buffer_size, "ls -l %s", dir_path);

    // Open the pipe to the command's output
    fp = popen(buffer, "r");
    if (fp == NULL) {
        // Return an empty string on error
        return "";
    }

    // Read the command's output into a dynamically allocated buffer
    char *result = (char *)malloc(buffer_size);
    if (fgets(result, buffer_size, fp) != NULL) {
        // Close the pipe
        pclose(fp);
        return result;
    } else {
        // Free the allocated memory and return an empty string on error
        free(result);
        pclose(fp);
        return "";
    }
}
```