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

    // Construct the full path to the file
    size_t path_len = strlen(directory) + strlen(filename) + 2; // +1 for '/' and +1 for '\0'
    char *path = malloc(path_len);
    if (!path) {
        return;
    }
    snprintf(path, path_len, "%s/%s", directory, filename);

    // Open the file in write mode with restricted permissions
    FILE *file = fopen(path, "w");
    if (!file) {
        free(path);
        return;
    }

    // Write the content to the file
    fprintf(file, "%s", content);

    // Close the file
    fclose(file);

    // Set the file permissions to read/write only for the owner
    chmod(path, SECRET_FILE_MODE);

    // Free the allocated memory
    free(path);
}
```