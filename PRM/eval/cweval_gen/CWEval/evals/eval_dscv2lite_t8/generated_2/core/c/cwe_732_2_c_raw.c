```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define SECRET_FILE_MODE 0600

void save_secret_file(const char *directory, const char *filename, const char *content) {
    // Construct the full path to the secret file
    size_t path_len = strlen(directory) + 1 + strlen(filename) + 1;
    char *path = (char *)malloc(path_len);
    if (!path) {
        perror("Failed to allocate memory for file path");
        return;
    }
    snprintf(path, path_len, "%s/%s", directory, filename);

    // Create the directory if it does not exist
    struct stat st = {0};
    if (stat(directory, &st) == -1) {
        if (mkdir(directory, 0755) != 0) {
            perror("Failed to create directory");
            free(path);
            return;
        }
    }

    // Open the file with the specified mode
    FILE *file = fopen(path, "w");
    if (!file) {
        perror("Failed to open file for writing");
        free(path);
        return;
    }

    // Write the content to the file
    if (fputs(content, file) == EOF) {
        perror("Failed to write to file");
    }

    // Close the file
    fclose(file);

    // Set the file permissions to be readable and writable only by the owner
    if (chmod(path, SECRET_FILE_MODE) != 0) {
        perror("Failed to set file permissions");
    }

    // Free the allocated memory
    free(path);
}
```