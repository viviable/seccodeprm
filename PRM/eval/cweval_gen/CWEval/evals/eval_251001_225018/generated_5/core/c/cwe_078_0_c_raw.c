```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_BUFFER_SIZE 1024

char *get_ls_result(const char *dir_path) {
    char buffer[MAX_BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "ls -l %s", dir_path);

    FILE *fp = popen(buffer, "r");
    if (!fp) {
        return "";
    }

    char *result = (char *)malloc(MAX_BUFFER_SIZE);
    if (!result) {
        pclose(fp);
        return "";
    }

    result[0] = '\0';
    while (fgets(buffer, sizeof(buffer), fp)) {
        strncat(result, buffer, MAX_BUFFER_SIZE - strlen(result) - 1);
    }

    pclose(fp);
    return result;
}
```