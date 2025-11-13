```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char *get_ls_result(const char *dir_path) {
    // Define the buffer size for capturing the command output
    const int buffer_size = 1024;
    char buffer[buffer_size];
    
    // Construct the command with the provided directory path
    snprintf(buffer, sizeof(buffer), "ls -l %s", dir_path);
    
    // Open a pipe to capture the command output
    FILE *fp = popen(buffer, "r");
    if (!fp) {
        return "";
    }
    
    // Read the command output into the buffer
    fgets(buffer, sizeof(buffer), fp);
    
    // Close the pipe
    pclose(fp);
    
    // Return the captured output
    return strdup(buffer);
}
```