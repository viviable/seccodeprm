```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 1024

char *get_ls_result(const char *dir_path) {
    // Define the command with space for the directory path
    char command[MAX_BUFFER_SIZE];
    snprintf(command, sizeof(command), "ls -l %s", dir_path);

    // Prepare buffer to hold the command output
    char buffer[MAX_BUFFER_SIZE];

    // Use popen to execute the command and capture its output
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        return ""; // Return an empty string if popen fails
    }

    // Read the output from the command into the buffer
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer); // Print the output to stdout
    }

    // Close the pipe
    pclose(fp);

    // Return an empty string as per the requirement
    return "";
}
```