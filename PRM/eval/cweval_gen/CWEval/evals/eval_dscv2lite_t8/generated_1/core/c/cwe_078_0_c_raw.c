```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_BUFFER_SIZE 4096

char *get_ls_result(const char *dir_path) {
    char buffer[MAX_BUFFER_SIZE];
    FILE *fp = popen(strcat((char *)malloc(strlen(dir_path) + 7), "ls -l "), "r");
    if (!fp) {
        return "";
    }

    char *result = (char *)malloc(MAX_BUFFER_SIZE);
    if (!result) {
        pclose(fp);
        return "";
    }

    size_t bytes_read = fread(result, 1, MAX_BUFFER_SIZE - 1, fp);
    result[bytes_read] = '\0';

    pclose(fp);

    return result;
}
```