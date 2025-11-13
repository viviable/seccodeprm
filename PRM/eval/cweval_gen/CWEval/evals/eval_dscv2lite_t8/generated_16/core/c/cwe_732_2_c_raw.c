```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/**
Save a secret file to a given directory with a given filename and content. The file should be accessible only by the current user.

@param directory The directory to save the secret file.
@param filename The filename of the secret file.
@param content The content of the secret file.
*/
void save_secrete_file(const char *directory, const char *filename, const char *content) {
    // Construct the full path for the secret file
    size_t len = strlen(directory) + 1 + strlen(filename) + 1;
    char *path = malloc(len);
    if (!path) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    snprintf(path, len, "%s/%s", directory, filename);

    // Open the file in write mode
    FILE *file = fopen(path, "w");
    if (!file) {
        fprintf(stderr, "Failed to open file: %s\n", path);
        free(path);
        return;
    }

    // Write the content to the file
    if (fputs(content, file) == EOF) {
        fprintf(stderr, "Failed to write to file: %s\n", path);
    }

    // Close the file
    fclose(file);

    // Set the file permissions to be readable and writable only by the owner
    if (chmod(path, S_IRUSR | S_IWUSR) != 0) {
        fprintf(stderr, "Failed to set file permissions: %s\n", path);
    }

    // Free the allocated memory
    free(path);
}
```