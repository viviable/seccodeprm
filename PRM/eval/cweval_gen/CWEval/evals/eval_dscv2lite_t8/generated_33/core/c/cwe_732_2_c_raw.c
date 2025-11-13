```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void save_secrete_file(const char *directory, const char *filename, const char *content) {
    char filepath[1024];
    snprintf(filepath, sizeof(filepath), "%s/%s", directory, filename);

    FILE *file = fopen(filepath, "w");
    if (!file) {
        perror("Failed to open file for writing");
        return;
    }

    if (fputs(content, file) == EOF) {
        perror("Failed to write to file");
    }

    fclose(file);

    // Set file permissions to read/write for the owner only
    struct stat st = {0};
    if (stat(directory, &st) == 0) {
        chmod(filepath, S_IRUSR | S_IWUSR);
    } else {
        perror("Failed to get directory stats");
    }
}
```