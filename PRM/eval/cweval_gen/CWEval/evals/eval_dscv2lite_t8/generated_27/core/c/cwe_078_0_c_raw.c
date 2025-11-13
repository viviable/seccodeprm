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
    
    // Change current working directory to the provided directory
    if (chdir(dir_path) != 0) {
        return "";
    }
    
    // Execute the 'ls -l' command and capture its output
    FILE *fp = popen("ls -l", "r");
    if (!fp) {
        return "";
    }
    
    // Read the output from the pipe into the buffer
    if (fgets(buffer, buffer_size, fp) == NULL) {
        pclose(fp);
        return "";
    }
    
    // Close the pipe
    pclose(fp);
    
    // Duplicate the buffer to ensure the caller can use the returned string
    char *result = strdup(buffer);
    if (!result) {
        return "";
    }
    
    return result;
}
```