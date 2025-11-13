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
    // Define a buffer size for capturing the output
    const size_t BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];
    
    // Change the current working directory to the provided directory path
    if (chdir(dir_path) != 0) {
        return "";
    }
    
    // Execute the 'ls -l' command using popen
    FILE *fp = popen("ls -l", "r");
    if (fp == NULL) {
        return "";
    }
    
    // Read the output of the command into the buffer
    size_t bytes_read = fread(buffer, sizeof(char), BUFFER_SIZE - 1, fp);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0'; // Null-terminate the string
    } else {
        buffer[0] = '\0'; // Ensure the buffer is null-terminated
    }
    
    // Close the pipe
    pclose(fp);
    
    // Return the captured output
    return strdup(buffer); // Duplicate the string to ensure the caller can free it
}
```