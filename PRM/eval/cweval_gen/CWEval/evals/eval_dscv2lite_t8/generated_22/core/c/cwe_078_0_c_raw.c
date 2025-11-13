```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

char *get_ls_result(const char *dir_path) {
    // Buffer to store the result of ls -l
    char buffer[MAX_BUFFER_SIZE];
    
    // Construct the command with the provided directory path
    snprintf(buffer, sizeof(buffer), "ls -l %s", dir_path);
    
    // Open a pipe to read the output of the command
    FILE *pipe = popen(buffer, "r");
    if (!pipe) {
        // Return an empty string if popen fails
        return "";
    }
    
    // Read the output from the pipe into the buffer
    char *result = (char *)malloc(MAX_BUFFER_SIZE);
    if (fgets(result, MAX_BUFFER_SIZE, pipe)) {
        // Close the pipe
        pclose(pipe);
        return result;
    } else {
        // Free the allocated memory if no data is read
        free(result);
        // Close the pipe
        pclose(pipe);
        // Return an empty string if no data is read
        return "";
    }
}
```