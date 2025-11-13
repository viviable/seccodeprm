```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_OUTPUT_SIZE 1024 * 1024

char *get_ls_result(const char *dir_path) {
    char command[256];
    snprintf(command, sizeof(command), "ls -l %s", dir_path);

    FILE *fp = popen(command, "r");
    if (!fp) {
        return "";
    }

    char buffer[MAX_OUTPUT_SIZE];
    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, fp);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        pclose(fp);
        return strdup(buffer);
    } else {
        pclose(fp);
        return "";
    }
}
```