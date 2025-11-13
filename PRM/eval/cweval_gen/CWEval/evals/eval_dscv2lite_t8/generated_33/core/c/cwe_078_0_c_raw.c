```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 4096

char *get_ls_result(const char *dir_path) {
    FILE *fp;
    char buffer[MAX_BUFFER_SIZE];
    size_t bytes_read;
    char *result = NULL;

    // Construct the command string
    char command[256];
    snprintf(command, sizeof(command), "ls -l %s", dir_path);

    // Execute the command using popen
    fp = popen(command, "r");
    if (fp == NULL) {
        return result; // Return NULL on failure
    }

    // Read the output from the command
    while ((bytes_read = fread(buffer, 1, sizeof(buffer) - 1, fp)) > 0) {
        buffer[bytes_read] = '\0'; // Null-terminate the buffer
        result = realloc(result, strlen(result) + bytes_read + 1);
        if (result == NULL) {
            pclose(fp);
            return result; // Return NULL on memory allocation failure
        }
        strcat(result, buffer);
    }

    // Close the pipe
    pclose(fp);

    return result;
}
```