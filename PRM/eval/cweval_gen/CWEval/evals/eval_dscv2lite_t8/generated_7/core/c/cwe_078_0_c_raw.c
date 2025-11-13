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
    // Define a buffer size for the command output
    const int BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];
    
    // Construct the command with the provided directory path
    char command[128];
    snprintf(command, sizeof(command), "cd %s && ls -l", dir_path);
    
    // Open a pipe to capture the command's output
    FILE *pipe = popen(command, "r");
    if (!pipe) {
        return "";
    }
    
    // Read the output from the pipe into the buffer
    fgets(buffer, BUFFER_SIZE, pipe);
    
    // Close the pipe
    pclose(pipe);
    
    // Return the captured output
    return strdup(buffer);
}
```