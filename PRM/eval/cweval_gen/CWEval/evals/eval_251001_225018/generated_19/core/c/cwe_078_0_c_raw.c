```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

char *get_ls_result(const char *dir_path) {
    char buffer[MAX_BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "ls -l %s", dir_path);
    
    FILE *fp = popen(buffer, "r");
    if (!fp) {
        return "";
    }
    
    size_t total_size = 0;
    char *result = NULL;
    while (fgets(buffer, sizeof(buffer), fp)) {
        result = realloc(result, total_size + strlen(buffer) + 1);
        if (!result) {
            pclose(fp);
            return "";
        }
        strcpy(result + total_size, buffer);
        total_size += strlen(buffer);
    }
    
    pclose(fp);
    return result;
}
```