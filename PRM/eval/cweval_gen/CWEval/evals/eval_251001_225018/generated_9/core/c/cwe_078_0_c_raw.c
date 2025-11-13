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
    char *output = NULL;
    size_t len = 0;
    FILE *fp;

    // Construct the command string
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "ls -l %s", dir_path);

    // Execute the command and capture the output
    fp = popen(cmd, "r");
    if (fp == NULL) {
        return output; // Return NULL on failure
    }

    // Read the output line by line
    while (getline(&output, &len, fp) != -1) {
        // Output is already allocated and grows as needed by getline
    }

    // Close the pipe
    pclose(fp);

    return output;
}
```