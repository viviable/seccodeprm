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
    // Define the buffer size for capturing the output
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    
    // Change the current working directory to the provided path
    if (chdir(dir_path) != 0) {
        return "";
    }
    
    // Execute the ls -l command using popen
    FILE *fp = popen("ls -l", "r");
    if (fp == NULL) {
        return "";
    }
    
    // Read the output from the command
    if (fgets(buffer, sizeof(buffer), fp) == NULL) {
        pclose(fp);
        return "";
    }
    
    // Close the pipe
    pclose(fp);
    
    // Duplicate the buffer to return it
    char *result = strdup(buffer);
    if (result == NULL) {
        return "";
    }
    
    return result;
}
```