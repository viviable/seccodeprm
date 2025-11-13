```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

char *get_ls_result(const char *dir_path) {
    char buffer[MAX_BUFFER_SIZE];
    FILE *fp = popen("ls -l " dir_path, "r");
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