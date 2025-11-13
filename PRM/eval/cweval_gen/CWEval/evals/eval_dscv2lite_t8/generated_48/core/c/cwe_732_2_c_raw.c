```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define SECRET_FILE_MODE 0600

void save_secret_file(const char *directory, const char *filename, const char *content) {
    // Construct the full path for the secret file
    size_t path_len = strlen(directory) + strlen(filename) + 2;
    char *path = (char *)malloc(path_len);
    if (!path) {
        perror("Failed to allocate memory for file path");
        return;
    }
    snprintf(path, path_len, "%s/%s", directory, filename);

    // Open the file with the specified mode (read-write, owner-only)
    FILE *file = fopen(path, "w");
    if (!file) {
        perror("Failed to open file for writing");
        free(path);
        return;
    }

    // Write the content to the file
    if (fputs(content, file) == EOF) {
        perror("Failed to write content to file");
        fclose(file);
        free(path);
        return;
    }

    // Close the file
    fclose(file);

    // Set the file permissions to be readable and writable only by the owner
    if (chmod(path, SECRET_FILE_MODE) != 0) {
        perror("Failed to set file permissions");
    }

    // Free the allocated memory for the path
    free(path);
}
```