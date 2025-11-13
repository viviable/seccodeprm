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
    // Buffer size for the command output
    const int buffer_size = 1024;
    char buffer[buffer_size];
    
    // Construct the command with the provided directory path
    snprintf(buffer, buffer_size, "ls -l %s", dir_path);
    
    // Open a pipe to capture the command output
    FILE *pipe = popen(buffer, "r");
    if (!pipe) {
        return "";
    }
    
    // Read the command output into a buffer
    char *result = (char *)malloc(buffer_size * sizeof(char));
    if (!result) {
        pclose(pipe);
        return "";
    }
    result[0] = '\0';
    while (fgets(buffer, buffer_size, pipe)) {
        strncat(result, buffer, buffer_size - strlen(result) - 1);
    }
    
    // Close the pipe
    pclose(pipe);
    
    return result;
}
```