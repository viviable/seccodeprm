```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void save_secrete_file(const char *directory, const char *filename, const char *content) {
    // Construct the full path for the file
    size_t path_length = strlen(directory) + 1 + strlen(filename) + 1;
    char *path = malloc(path_length);
    if (!path) {
        perror("Failed to allocate memory for file path");
        return;
    }
    snprintf(path, path_length, "%s/%s", directory, filename);

    // Open the file in write mode
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
    if (chmod(path, S_IRUSR | S_IWUSR) != 0) {
        perror("Failed to set file permissions");
    }

    // Free the allocated memory
    free(path);
}
```