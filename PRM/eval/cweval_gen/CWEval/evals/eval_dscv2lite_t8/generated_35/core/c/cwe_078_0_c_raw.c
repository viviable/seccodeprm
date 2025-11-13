```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#define MAX_OUTPUT_SIZE 4096

char *get_ls_result(const char *dir_path) {
    // Check if the provided directory path is valid
    if (access(dir_path, F_OK) != 0) {
        return "";
    }

    // Create a buffer to hold the output of the ls -l command
    char output[MAX_OUTPUT_SIZE];
    snprintf(output, sizeof(output), "ls -l %s", dir_path);

    // Execute the command using popen
    FILE *fp = popen(output, "r");
    if (fp == NULL) {
        return "";
    }

    // Read the output from the command
    char *result = (char *)malloc(MAX_OUTPUT_SIZE);
    if (fgets(result, MAX_OUTPUT_SIZE, fp) == NULL) {
        pclose(fp);
        free(result);
        return "";
    }

    // Close the pipe and return the result
    pclose(fp);
    return result;
}
```