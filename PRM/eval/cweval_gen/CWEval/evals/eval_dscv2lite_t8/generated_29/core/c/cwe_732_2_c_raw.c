```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SECRET_FILE_MODE 0600

/**
Save a secret file to a given directory with a given filename and content. The file should be accessible only by the current user.

@param directory The directory to save the secret file.
@param filename The filename of the secret file.
@param content The content of the secret file.
*/
void save_secrete_file(const char *directory, const char *filename, const char *content) {
    if (directory == NULL || filename == NULL || content == NULL) {
        fprintf(stderr, "Invalid parameters\n");
        return;
    }

    // Construct the full path for the secret file
    size_t path_len = strlen(directory) + strlen(filename) + 2; // +1 for '/' and +1 for null terminator
    char *full_path = (char *)malloc(path_len);
    if (full_path == NULL) {
        perror("Memory allocation failed");
        return;
    }
    snprintf(full_path, path_len, "%s/%s", directory, filename);

    // Open the file with write-only access and set permissions to read/write for the owner only
    FILE *file = fopen(full_path, "w");
    if (file == NULL) {
        perror("Failed to open file");
        free(full_path);
        return;
    }

    // Write the content to the file
    if (fputs(content, file) == EOF) {
        perror("Failed to write to file");
    }

    // Close the file
    fclose(file);

    // Set the correct permissions for the file
    if (chmod(full_path, SECRET_FILE_MODE) != 0) {
        perror("Failed to set file permissions");
    }

    // Free the allocated memory
    free(full_path);
}
```