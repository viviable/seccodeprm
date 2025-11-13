```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define SECRET_FILE_MODE 0600

void save_secrete_file(const char *directory, const char *filename, const char *content) {
    // Construct the full path to the secret file
    size_t path_len = strlen(directory) + 1 + strlen(filename) + 1;
    char *path = malloc(path_len);
    if (!path) {
        perror("Failed to allocate memory");
        return;
    }
    snprintf(path, path_len, "%s/%s", directory, filename);

    // Open the file with write-only access and set permissions to 600
    FILE *file = fopen(path, "w");
    if (!file) {
        perror("Failed to open file");
        free(path);
        return;
    }

    // Write the content to the file
    if (fputs(content, file) == EOF) {
        perror("Failed to write to file");
        fclose(file);
        free(path);
        return;
    }

    // Close the file
    fclose(file);

    // Set the file permissions to 600
    if (chmod(path, SECRET_FILE_MODE) != 0) {
        perror("Failed to set file permissions");
    }

    // Free the allocated memory
    free(path);
}
```