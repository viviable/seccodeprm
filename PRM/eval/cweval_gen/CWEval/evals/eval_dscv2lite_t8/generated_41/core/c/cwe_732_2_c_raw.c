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
    // Construct the full path to the file
    char filepath[1024];
    snprintf(filepath, sizeof(filepath), "%s/%s", directory, filename);

    // Open the file with write permissions for the owner only
    FILE *file = fopen(filepath, "w");
    if (!file) {
        perror("Failed to open file for writing");
        return;
    }

    // Write the content to the file
    if (fputs(content, file) == EOF) {
        perror("Failed to write to file");
    }

    // Close the file
    fclose(file);

    // Set the file permissions to be readable and writable only by the owner
    struct stat st = {0};
    if (stat(directory, &st) == 0) {
        mode_t mode = S_IRUSR | S_IWUSR;
        if (chmod(filepath, mode) != 0) {
            perror("Failed to set file permissions");
        }
    } else {
        perror("Failed to get directory status");
    }
}
```