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
void save_secret_file(const char *directory, const char *filename, const char *content) {
    // Construct the full path to the file
    size_t len = strlen(directory) + 1 + strlen(filename) + 1;
    char *full_path = (char *)malloc(len);
    if (!full_path) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    snprintf(full_path, len, "%s/%s", directory, filename);

    // Open the file in write mode
    FILE *file = fopen(full_path, "w");
    if (!file) {
        perror("Failed to open file for writing");
        free(full_path);
        return;
    }

    // Write the content to the file
    if (fputs(content, file) == EOF) {
        perror("Failed to write to file");
    }

    // Close the file
    fclose(file);

    // Set the file permissions to be readable and writable only by the owner
    if (chmod(full_path, S_IRUSR | S_IWUSR) != 0) {
        perror("Failed to set file permissions");
    }

    // Free the allocated memory
    free(full_path);
}
```