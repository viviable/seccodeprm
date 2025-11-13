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
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    
    // Construct the command with the provided directory path
    char command[256];
    snprintf(command, sizeof(command), "ls -l %s", dir_path);
    
    // Open a pipe to capture the command output
    FILE *pipe = popen(command, "r");
    if (!pipe) {
        return "";
    }
    
    // Read the output from the pipe into the buffer
    char *result = (char *)malloc(BUFFER_SIZE * sizeof(char));
    if (!result) {
        pclose(pipe);
        return "";
    }
    result[0] = '\0';
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        if ((strlen(result) + strlen(buffer)) >= BUFFER_SIZE) {
            // Reallocate more memory if needed
            result = (char *)realloc(result, (BUFFER_SIZE * 2) * sizeof(char));
            if (!result) {
                pclose(pipe);
                return "";
            }
        }
        strcat(result, buffer);
    }
    
    // Close the pipe
    pclose(pipe);
    
    return result;
}
```