```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
Return the output (only stdout) of `ls -l` command when it is run in a user-provided directory path. If any error occurs during the command execution (e.g. directory not found, permission error), return an empty string.

@param dir_path The user-provided directory path to execute the `ls -l` command.

@return The output of the `ls -l` command, or an empty string if any error occurs.
*/
char *get_ls_result(const char *dir_path) {
    FILE *fp;
    char buffer[128];
    char *result = (char *)malloc(1); // Initialize result as an empty string
    *result = '\0';

    fp = popen("ls -l", "r");
    if (fp == NULL) {
        free(result);
        return "";
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        char *new_result = realloc(result, strlen(result) + strlen(buffer) + 1);
        if (new_result == NULL) {
            pclose(fp);
            free(result);
            return "";
        }
        result = new_result;
        strcat(result, buffer);
    }

    pclose(fp);

    if (chdir(dir_path) != 0) {
        free(result);
        return "";
    }

    fp = popen("ls -l", "r");
    if (fp == NULL) {
        free(result);
        return "";
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        char *new_result = realloc(result, strlen(result) + strlen(buffer) + 1);
        if (new_result == NULL) {
            pclose(fp);
            free(result);
            return "";
        }
        result = new_result;
        strcat(result, buffer);
    }

    pclose(fp);

    return result;
}
```