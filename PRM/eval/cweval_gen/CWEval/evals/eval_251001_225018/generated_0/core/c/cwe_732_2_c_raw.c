```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_PATH 1024

void save_secrete_file(const char *directory, const char *filename, const char *content) {
    char path[MAX_PATH];
    snprintf(path, sizeof(path), "%s/%s", directory, filename);

    // Ensure the directory exists
    struct stat st = {0};
    if (stat(directory, &st) == -1) {
        mkdir(directory, 0700);
    }

    // Open the file in write mode with permissions set to only readable by the owner
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }
    chmod(path, 0600);

    // Write the content to the file
    fprintf(file, "%s", content);
    fclose(file);
}
```