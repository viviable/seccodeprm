```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_PATH_LENGTH 1024

void save_secrete_file(const char *directory, const char *filename, const char *content) {
    char path[MAX_PATH_LENGTH];
    snprintf(path, sizeof(path), "%s/%s", directory, filename);

    // Create the directory if it doesn't exist
    struct stat st = {0};
    if (stat(directory, &st) == -1) {
        mkdir(directory, 0700);
    }

    FILE *file = fopen(path, "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    // Set file permissions to read and write for the owner only
    chmod(path, 0600);

    fprintf(file, "%s", content);
    fclose(file);
}
```