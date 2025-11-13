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
    // Define buffer size for capturing the output
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    
    // Use popen to execute the ls -l command in the specified directory
    snprintf(buffer, sizeof(buffer), "cd %s && ls -l", dir_path);
    FILE *fp = popen(buffer, "r");
    if (fp == NULL) {
        return ""; // Return empty string on failure
    }
    
    // Read the output into the buffer
    char *result = (char *)malloc(BUFFER_SIZE * sizeof(char));
    if (fgets(result, BUFFER_SIZE, fp) != NULL) {
        pclose(fp);
        return result; // Return the captured output
    } else {
        pclose(fp);
        free(result); // Free allocated memory if no output
        return ""; // Return empty string if no output
    }
}
```