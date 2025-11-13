```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_OUTPUT_SIZE 1024

char *get_ls_result(const char *dir_path) {
    pid_t pid;
    int status;
    char *output = (char *)malloc(MAX_OUTPUT_SIZE);
    FILE *fp;

    if (output == NULL) {
        return "";
    }

    // Construct the command with the provided directory path
    char command[MAX_OUTPUT_SIZE];
    snprintf(command, sizeof(command), "ls -l %s", dir_path);

    // Execute the command using popen
    fp = popen(command, "r");
    if (fp == NULL) {
        free(output);
        return "";
    }

    // Read the output from the command
    while (fgets(output + strlen(output), MAX_OUTPUT_SIZE - strlen(output) - 1, fp) != NULL) {
        // Check if the buffer is full
        if (strlen(output) >= MAX_OUTPUT_SIZE - 1) {
            break;
        }
    }

    // Close the pipe
    pclose(fp);

    // Wait for the process to finish
    waitpid(pid, &status, 0);

    // Return the output
    return output;
}
```
This function uses `popen` to execute the `ls -l` command in the specified directory and captures its output. It constructs the command string dynamically based on the input directory path. The function handles memory allocation for the output buffer and checks for errors during the execution of the command. If successful, it returns the captured output; otherwise, it returns an empty string.