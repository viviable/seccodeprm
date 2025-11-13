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
    const size_t BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];
    
    // Change to the provided directory
    if (chdir(dir_path) != 0) {
        return "";
    }
    
    // Execute ls -l command using popen
    FILE *fp = popen("ls -l", "r");
    if (!fp) {
        return "";
    }
    
    // Read the output of the command
    if (fgets(buffer, BUFFER_SIZE, fp) == NULL) {
        pclose(fp);
        return "";
    }
    
    // Close the pipe
    pclose(fp);
    
    // Duplicate the buffer to ensure the caller can free the memory
    char *result = strdup(buffer);
    if (!result) {
        return "";
    }
    
    return result;
}
```