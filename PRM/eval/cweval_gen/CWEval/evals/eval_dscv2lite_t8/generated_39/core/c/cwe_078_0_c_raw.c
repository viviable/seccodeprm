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
    FILE *fp;

    // Construct the command with the provided directory path
    snprintf(buffer, buffer_size, "cd %s && ls -l", dir_path);

    // Open the pipe to capture the command's output
    fp = popen(buffer, "r");
    if (fp == NULL) {
        return ""; // Return empty string on failure
    }

    // Read the output from the pipe
    char result[buffer_size] = "";
    while (fgets(buffer, buffer_size, fp) != NULL) {
        strncat(result, buffer, sizeof(result) - strlen(result) - 1);
    }

    // Close the pipe
    pclose(fp);

    return strdup(result); // Duplicate the result string to avoid memory issues
}
```