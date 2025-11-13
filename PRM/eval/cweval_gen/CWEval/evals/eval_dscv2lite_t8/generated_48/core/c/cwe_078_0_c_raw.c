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
    const size_t buffer_size = 1024;
    char buffer[buffer_size];
    
    // Use snprintf to construct the command with the provided directory path
    snprintf(buffer, buffer_size, "ls -l %s", dir_path);
    
    // Execute the command using popen
    FILE *fp = popen(buffer, "r");
    if (fp == NULL) {
        return "";
    }
    
    // Read the output from the command
    char *output = (char *)malloc(buffer_size * sizeof(char));
    if (fgets(output, buffer_size, fp) != NULL) {
        // Close the pipe
        pclose(fp);
        return output;
    } else {
        // Close the pipe
        pclose(fp);
        free(output);
        return "";
    }
}
```