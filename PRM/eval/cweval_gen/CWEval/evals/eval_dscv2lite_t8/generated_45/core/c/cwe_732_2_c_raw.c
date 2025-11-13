```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define SECRET_FILE_MODE 0600

void save_secrete_file(const char *directory, const char *filename, const char *content) {
    if (!directory || !filename || !content) {
        return;
    }

    // Construct full path to the file
    size_t path_len = strlen(directory) + 1 + strlen(filename) + 1;
    char *path = (char *)malloc(path_len);
    if (!path) {
        return;
    }
    snprintf(path, path_len, "%s/%s", directory, filename);

    // Create directory if it doesn't exist
    struct stat st = {0};
    if (stat(directory, &st) == -1) {
        mkdir(directory, 0755); // Directory permissions: rwxr-xr-x
    }

    // Open the file with write-only access and create if it doesn't exist
    FILE *file = fopen(path, "w");
    if (!file) {
        free(path);
        return;
    }

    // Write content to the file
    if (fputs(content, file) == EOF) {
        fclose(file);
        free(path);
        return;
    }

    // Close the file
    fclose(file);

    // Set file permissions to read and write only for the owner
    chmod(path, SECRET_FILE_MODE);

    // Free allocated memory
    free(path);
}
```