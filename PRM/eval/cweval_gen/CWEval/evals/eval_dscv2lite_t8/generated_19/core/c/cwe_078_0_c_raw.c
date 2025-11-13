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
    
    // Construct the command with the provided directory path
    snprintf(buffer, BUFFER_SIZE, "ls -l %s", dir_path);
    
    // Open a pipe to capture the command output
    FILE *fp = popen(buffer, "r");
    if (!fp) {
        return "";
    }
    
    // Read the output from the pipe into the buffer
    fgets(buffer, BUFFER_SIZE, fp);
    
    // Close the pipe
    pclose(fp);
    
    // Return the captured output
    return strdup(buffer);
}
```