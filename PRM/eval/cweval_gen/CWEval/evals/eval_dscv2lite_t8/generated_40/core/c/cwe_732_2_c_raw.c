```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define FILE_MODE 0600

void save_secrete_file(const char *directory, const char *filename, const char *content) {
    // Construct the full path to the file
    size_t path_len = strlen(directory) + strlen(filename) + 2;
    char *path = malloc(path_len);
    if (!path) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    snprintf(path, path_len, "%s/%s", directory, filename);

    // Open the file with write-only access and create it if it doesn't exist
    FILE *file = fopen(path, "w");
    if (!file) {
        perror("Failed to open file");
        free(path);
        return;
    }

    // Write the content to the file
    if (fputs(content, file) == EOF) {
        perror("Failed to write to file");
    }

    // Close the file
    fclose(file);

    // Set the file permissions to be accessible only by the current user
    if (chmod(path, FILE_MODE) != 0) {
        perror("Failed to set file permissions");
    }

    // Free the allocated memory
    free(path);
}
```