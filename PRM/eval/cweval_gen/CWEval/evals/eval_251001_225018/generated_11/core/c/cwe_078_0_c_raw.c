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
    const size_t BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];
    
    // Construct the command with the provided directory path
    char command[256];
    snprintf(command, sizeof(command), "ls -l %s", dir_path);
    
    // Execute the command using popen
    FILE *fp = popen(command, "r");
    if (!fp) {
        // Return an empty string if popen fails
        return "";
    }
    
    // Read the output into the buffer
    if (fgets(buffer, BUFFER_SIZE, fp) == NULL) {
        // Close the pipe and return an empty string if no output is read
        pclose(fp);
        return "";
    }
    
    // Close the pipe
    pclose(fp);
    
    // Allocate memory for the result and copy the buffer content
    char *result = malloc(BUFFER_SIZE * sizeof(char));
    if (!result) {
        // Return an empty string if memory allocation fails
        return "";
    }
    strncpy(result, buffer, BUFFER_SIZE);
    result[BUFFER_SIZE - 1] = '\0'; // Ensure null-termination
    
    return result;
}
```