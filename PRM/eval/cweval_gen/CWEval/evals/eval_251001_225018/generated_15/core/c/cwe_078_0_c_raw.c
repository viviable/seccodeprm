```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

char *get_ls_result(const char *dir_path) {
    // Construct the command with the provided directory path
    char command[MAX_BUFFER_SIZE];
    snprintf(command, sizeof(command), "ls -l %s", dir_path);

    // Prepare buffer to hold the result
    char *result = (char *)malloc(MAX_BUFFER_SIZE);
    if (!result) {
        return "";
    }

    // Open pipe to capture the output of the command
    FILE *pipe = popen(command, "r");
    if (!pipe) {
        free(result);
        return "";
    }

    // Read the output from the pipe into the buffer
    if (fgets(result, MAX_BUFFER_SIZE, pipe) != NULL) {
        // Close the pipe
        pclose(pipe);
        return result;
    } else {
        // Close the pipe
        pclose(pipe);
        // Free allocated memory since no data was read
        free(result);
        return "";
    }
}
```