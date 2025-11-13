```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

void save_secrete_file(const char *directory, const char *filename, const char *content) {
    // Construct the full path to the file
    size_t path_len = strlen(directory) + 1 + strlen(filename) + 1;
    char *path = malloc(path_len);
    if (!path) {
        perror("Failed to allocate memory");
        return;
    }
    snprintf(path, path_len, "%s/%s", directory, filename);

    // Open the file with write permissions for the owner only
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
    mode_t mode = S_IRUSR | S_IWUSR;
    if (chmod(path, mode) != 0) {
        perror("Failed to set file permissions");
    }

    // Free the allocated memory
    free(path);
}
```