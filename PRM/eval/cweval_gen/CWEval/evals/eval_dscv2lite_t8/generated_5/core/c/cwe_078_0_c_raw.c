```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_BUFFER_SIZE 1024

char *get_ls_result(const char *dir_path) {
    // Allocate memory for the result buffer
    char *result = (char *)malloc(MAX_BUFFER_SIZE);
    if (!result) {
        return "";
    }

    // Initialize the result buffer with null character
    result[0] = '\0';

    // Prepare the command to be executed
    char command[MAX_BUFFER_SIZE];
    snprintf(command, sizeof(command), "ls -l %s", dir_path);

    // Execute the command using popen
    FILE *fp = popen(command, "r");
    if (!fp) {
        free(result);
        return "";
    }

    // Read the output from the command into the result buffer
    while (fgets(result + strlen(result), MAX_BUFFER_SIZE - strlen(result), fp)) {
        // Ensure the buffer does not overflow
        if (strlen(result) >= MAX_BUFFER_SIZE - 1) {
            break;
        }
    }

    // Close the pipe
    pclose(fp);

    return result;
}
```