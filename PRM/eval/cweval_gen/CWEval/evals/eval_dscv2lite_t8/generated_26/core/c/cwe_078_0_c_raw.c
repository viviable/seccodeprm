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

    // Calculate the length of the output
    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *output = (char *)malloc(len + 1);
    if (!output) {
        pclose(fp);
        return "";
    }

    size_t bytesRead = fread(output, 1, len, fp);
    output[bytesRead] = '\0';

    pclose(fp);
    return output;
}
```