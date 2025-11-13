```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char *get_ls_result(const char *dir_path) {
    // Define a buffer size for capturing the output
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    
    // Construct the command with the provided directory path
    snprintf(buffer, sizeof(buffer), "ls -l %s", dir_path);
    
    // Open a pipe to capture the output of the command
    FILE *fp = popen(buffer, "r");
    if (fp == NULL) {
        // Return an empty string if popen fails
        return "";
    }
    
    // Read the output from the pipe into a buffer
    char result[BUFFER_SIZE] = "";
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        strncat(result, buffer, sizeof(result) - strlen(result) - 1);
    }
    
    // Close the pipe
    pclose(fp);
    
    // Return the captured output
    return strdup(result); // Use strdup to allocate memory for the returned string
}
```
This implementation uses `popen` to execute the `ls -l` command in the specified directory and captures its output. It reads the output line by line into a buffer and then returns a dynamically allocated copy of the result using `strdup`. If any errors occur during the process, such as failing to open the pipe or read the output, the function returns an empty string.