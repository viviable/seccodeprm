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
    FILE *fp;
    char buffer[128];
    char *result = (char *)malloc(0);
    size_t result_size = 0;

    // Construct the command with the provided directory path
    char command[256];
    snprintf(command, sizeof(command), "ls -l %s", dir_path);

    // Open the pipe to the command
    fp = popen(command, "r");
    if (fp == NULL) {
        return "";
    }

    // Read the output from the command
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        result = realloc(result, result_size + strlen(buffer) + 1);
        strcpy(result + result_size, buffer);
        result_size += strlen(buffer);
    }

    // Close the pipe
    pclose(fp);

    return result;
}
```