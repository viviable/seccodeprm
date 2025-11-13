```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_OUTPUT_SIZE 4096

char *get_ls_result(const char *dir_path) {
    char *output = malloc(MAX_OUTPUT_SIZE);
    if (!output) {
        return "";
    }

    // Construct the command with the provided directory path
    char command[MAX_OUTPUT_SIZE];
    snprintf(command, sizeof(command), "ls -l %s", dir_path);

    // Execute the command using popen
    FILE *fp = popen(command, "r");
    if (!fp) {
        free(output);
        return "";
    }

    // Read the output from the command
    if (fgets(output, MAX_OUTPUT_SIZE, fp) == NULL) {
        pclose(fp);
        free(output);
        return "";
    }

    // Close the pipe
    pclose(fp);

    return output;
}
```