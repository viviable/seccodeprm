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
    // Define the buffer size
    const size_t buffer_size = 1024;
    // Allocate memory for the buffer
    char *buffer = (char *)malloc(buffer_size);
    if (!buffer) {
        return "";
    }

    // Use snprintf to safely format the command with the provided directory path
    snprintf(buffer, buffer_size, "ls -l %s", dir_path);

    // Open a pipe to capture the output of the command
    FILE *fp = popen(buffer, "r");
    if (!fp) {
        free(buffer);
        return "";
    }

    // Read the output from the pipe into the buffer
    fgets(buffer, buffer_size, fp);

    // Close the pipe
    pclose(fp);

    return buffer;
}
```