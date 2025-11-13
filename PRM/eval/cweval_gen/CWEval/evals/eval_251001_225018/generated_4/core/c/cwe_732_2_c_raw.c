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
    // Construct the full path for the file
    size_t path_len = strlen(directory) + 1 + strlen(filename) + 1;
    char *path = (char *)malloc(path_len);
    if (path == NULL) {
        perror("Failed to allocate memory");
        return;
    }
    snprintf(path, path_len, "%s/%s", directory, filename);

    // Open the file in write mode
    FILE *file = fopen(path, "w");
    if (file == NULL) {
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