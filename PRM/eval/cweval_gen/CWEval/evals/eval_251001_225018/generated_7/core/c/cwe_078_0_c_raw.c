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
    // Define buffer size for the command output
    const size_t BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    
    // Format the command with the provided directory path
    snprintf(buffer, BUFFER_SIZE, "cd %s && ls -l", dir_path);
    
    // Execute the command using popen
    FILE *fp = popen(buffer, "r");
    if (fp == NULL) {
        return "";
    }
    
    // Read the output from the command
    char *result = (char *)malloc(BUFFER_SIZE * sizeof(char));
    if (fgets(result, BUFFER_SIZE, fp) != NULL) {
        // Remove trailing newline character if present
        size_t len = strlen(result);
        if (len > 0 && result[len - 1] == '\n') {
            result[len - 1] = '\0';
        }
    } else {
        free(result);
        result = "";
    }
    
    // Close the pipe
    pclose(fp);
    
    return result;
}
```