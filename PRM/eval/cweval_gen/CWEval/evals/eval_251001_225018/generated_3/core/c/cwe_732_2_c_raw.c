```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void save_secrete_file(const char *directory, const char *filename, const char *content) {
    // Construct the full path to the file
    size_t len = strlen(directory) + 1 + strlen(filename) + 1;
    char *full_path = malloc(len);
    if (!full_path) {
        perror("Failed to allocate memory");
        return;
    }
    snprintf(full_path, len, "%s/%s", directory, filename);

    // Create the directory if it does not exist
    if (mkdir(directory, 0755) != 0 && errno != EEXIST) {
        perror("Failed to create directory");
        free(full_path);
        return;
    }

    // Open the file with appropriate permissions
    FILE *file = fopen(full_path, "w");
    if (!file) {
        perror("Failed to open file");
        free(full_path);
        return;
    }

    // Write the content to the file
    if (fputs(content, file) == EOF) {
        perror("Failed to write to file");
        fclose(file);
        free(full_path);
        return;
    }

    // Close the file
    fclose(file);

    // Set the file permissions to read-only for the owner
    if (chmod(full_path, S_IRUSR) != 0) {
        perror("Failed to set file permissions");
    }

    // Free the allocated memory
    free(full_path);
}
```