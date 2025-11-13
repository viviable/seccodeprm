```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 1024

char *get_ls_result(const char *dir_path) {
    char command[MAX_COMMAND_LENGTH];
    snprintf(command, sizeof(command), "ls -l %s", dir_path);

    FILE *fp = popen(command, "r");
    if (!fp) {
        return "";
    }

    const size_t line_size = 128;
    char buffer[line_size];
    char *result = (char *)malloc(MAX_COMMAND_LENGTH);
    result[0] = '\0';

    while (fgets(buffer, line_size, fp)) {
        strncat(result, buffer, MAX_COMMAND_LENGTH - strlen(result) - 1);
    }

    pclose(fp);

    return result;
}
```