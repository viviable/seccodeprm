```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define SECRET_FILE_MODE 0600

void save_secrete_file(const char *directory, const char *filename, const char *content) {
    if (directory == NULL || filename == NULL || content == NULL) {
        fprintf(stderr, "Error: Null parameter provided.\n");
        return;
    }

    // Construct the full path for the secret file
    size_t path_len = strlen(directory) + strlen(filename) + 2; // +1 for '/' and +1 for null terminator
    char *path = malloc(path_len);
    if (path == NULL) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return;
    }
    snprintf(path, path_len, "%s/%s", directory, filename);

    // Create the directory if it does not exist
    struct stat st = {0};
    if (stat(directory, &st) == -1) {
        if (mkdir(directory, 0755) != 0) {
            perror("Error creating directory");
            free(path);
            return;
        }
    }

    // Open the file with write-only access, create if it doesn't exist, and set permissions
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        perror("Error opening file");
        free(path);
        return;
    }

    // Write the content to the file
    if (fputs(content, file) == EOF) {
        perror("Error writing to file");
    }

    // Close the file
    fclose(file);

    // Set the file permissions to read/write for the owner only
    if (chmod(path, SECRET_FILE_MODE) != 0) {
        perror("Error setting file permissions");
    }

    // Free allocated memory
    free(path);
}
```