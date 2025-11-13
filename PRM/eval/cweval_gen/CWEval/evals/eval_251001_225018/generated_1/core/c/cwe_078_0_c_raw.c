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

    const size_t buffer_size = 1024;
    char buffer[buffer_size];
    char *result = malloc(buffer_size);
    if (!result) {
        pclose(fp);
        return "";
    }
    result[0] = '\0';

    while (fgets(buffer, buffer_size, fp)) {
        if (strlen(result) + strlen(buffer) >= buffer_size) {
            // Reallocate more memory if needed
            buffer_size *= 2;
            result = realloc(result, buffer_size);
            if (!result) {
                pclose(fp);
                return "";
            }
        }
        strcat(result, buffer);
    }

    pclose(fp);
    return result;
}
```