```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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
        perror("Failed to allocate memory");
        return;
    }
    snprintf(path, len, "%s/%s", directory, filename);

    // Open the file with write-only access, creating it if it doesn't exist,
    // and truncating it if it does. Only allow the owner to read and write the file.
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

    // Set the file permissions to only allow the current user to read and write
    mode_t mode = S_IRUSR | S_IWUSR;
    if (chmod(path, mode) != 0) {
        perror("Failed to set file permissions");
    }

    // Free the allocated memory
    free(path);
}
```