```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 4096

char *get_ls_result(const char *dir_path) {
    // Construct the command with the provided directory path
    char command[MAX_BUFFER_SIZE];
    snprintf(command, sizeof(command), "ls -l %s", dir_path);

    // Prepare buffer to hold the result
    char result[MAX_BUFFER_SIZE] = {0};

    // Execute the command using popen
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        // Error occurred while opening the pipe
        return "";
    }

    // Read the output of the command into the buffer
    while (fgets(result + strlen(result), sizeof(result) - strlen(result), fp) != NULL) {
        // Check if the buffer is about to overflow
        if (strlen(result) >= sizeof(result) - 1) {
            pclose(fp);
            return "";
        }
    }

    // Close the pipe
    pclose(fp);

    // Return the result
    return result;
}
```