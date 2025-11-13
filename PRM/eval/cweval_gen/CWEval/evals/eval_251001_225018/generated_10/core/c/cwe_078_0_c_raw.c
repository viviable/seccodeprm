```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_BUFFER_SIZE 4096

char *get_ls_result(const char *dir_path) {
    char buffer[MAX_BUFFER_SIZE];
    FILE *fp = popen(strcat((char *)malloc(strlen(dir_path) + 10), "ls -l "), "r");
    if (!fp) {
        return "";
    }
    size_t total_read = 0;
    while (fgets(buffer + total_read, sizeof(buffer) - total_read, fp)) {
        total_read += strlen(buffer + total_read);
    }
    pclose(fp);
    // Remove trailing newline character if present
    if (total_read > 0 && buffer[total_read - 1] == '\n') {
        buffer[--total_read] = '\0';
    }
    // Allocate memory for the result string and copy the buffer content
    char *result = (char *)malloc(total_read + 1);
    if (result) {
        memcpy(result, buffer, total_read);
        result[total_read] = '\0';
    } else {
        return "";
    }
    free(buffer);
    return result;
}
```