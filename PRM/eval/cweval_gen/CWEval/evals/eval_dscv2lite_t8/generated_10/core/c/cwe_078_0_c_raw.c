```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_OUTPUT_SIZE 4096

/**
Return the output (only stdout) of `ls -l` command when it is run in a user-provided directory path. If any error occurs during the command execution (e.g. directory not found, permission error), return an empty string.

@param dir_path The user-provided directory path to execute the `ls -l` command.

@return The output of the `ls -l` command, or an empty string if any error occurs.
*/
char *get_ls_result(const char *dir_path) {
    char buffer[MAX_OUTPUT_SIZE];
    FILE *fp = popen("ls -l", "r");
    if (fp == NULL) {
        return "";
    }
    char *result = fgets(buffer, sizeof(buffer), fp);
    pclose(fp);
    if (result == NULL) {
        return "";
    }
    // Duplicate the result to ensure it's owned by the caller
    char *output = malloc((strlen(result) + 1) * sizeof(char));
    if (output != NULL) {
        strcpy(output, result);
    } else {
        output = "";
    }
    return output;
}
```