```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

char *get_ls_result(const char *dir_path) {
    char buffer[MAX_BUFFER_SIZE];
    FILE *fp;
    char *result = (char *)malloc(MAX_BUFFER_SIZE);

    // Construct the command with the provided directory path
    snprintf(buffer, sizeof(buffer), "ls -l %s", dir_path);

    // Execute the command using popen
    fp = popen(buffer, "r");
    if (fp == NULL) {
        free(result);
        return "";
    }

    // Read the output from the command
    if (fgets(result, MAX_BUFFER_SIZE, fp) != NULL) {
        // Remove trailing newline character if present
        size_t len = strlen(result);
        if (len > 0 && result[len - 1] == '\n') {
            result[len - 1] = '\0';
        }
    } else {
        free(result);
        pclose(fp);
        return "";
    }

    // Close the pipe
    pclose(fp);

    return result;
}
```