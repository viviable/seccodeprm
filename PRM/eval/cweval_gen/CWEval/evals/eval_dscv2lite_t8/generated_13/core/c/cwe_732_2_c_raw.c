```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define SECRET_FILE_MODE 0600

void save_secret_file(const char *directory, const char *filename, const char *content) {
    if (!directory || !filename || !content) {
        fprintf(stderr, "Invalid input parameters\n");
        return;
    }

    // Construct full path for the secret file
    size_t dir_len = strlen(directory);
    size_t file_len = strlen(filename);
    char *full_path = malloc(dir_len + file_len + 2); // +1 for '/' and +1 for '\0'
    if (!full_path) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    snprintf(full_path, dir_len + file_len + 2, "%s/%s", directory, filename);

    // Create the directory if it doesn't exist
    struct stat st = {0};
    if (stat(directory, &st) == -1) {
        if (mkdir(directory, 0755) != 0) {
            perror("Failed to create directory");
            free(full_path);
            return;
        }
    }

    // Open the file in write mode
    FILE *file = fopen(full_path, "w");
    if (!file) {
        perror("Failed to open file");
        free(full_path);
        return;
    }

    // Write content to the file
    if (fputs(content, file) == EOF) {
        perror("Failed to write to file");
    }

    // Close the file
    fclose(file);

    // Change file permissions to be readable and writable only by the owner
    if (chmod(full_path, SECRET_FILE_MODE) != 0) {
        perror("Failed to change file permissions");
    }

    // Free allocated memory
    free(full_path);
}
```