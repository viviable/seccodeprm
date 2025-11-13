```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define SECRET_FILE_PERMISSIONS 0600

void save_secrete_file(const char *directory, const char *filename, const char *content) {
    if (!directory || !filename || !content) {
        fprintf(stderr, "Invalid input parameters\n");
        return;
    }

    // Construct the full path to the secret file
    size_t path_len = strlen(directory) + strlen(filename) + 2; // +1 for '/' and +1 for null terminator
    char *file_path = (char *)malloc(path_len);
    if (!file_path) {
        perror("Failed to allocate memory for file path");
        return;
    }
    snprintf(file_path, path_len, "%s/%s", directory, filename);

    // Open the file with write-only access and set the permissions to 600 (read/write for owner only)
    FILE *file = fopen(file_path, "w");
    if (!file) {
        perror("Failed to open file for writing");
        free(file_path);
        return;
    }

    // Write the content to the file
    if (fputs(content, file) == EOF) {
        perror("Failed to write to file");
    } else {
        printf("Secret file saved successfully at %s\n", file_path);
    }

    // Close the file
    fclose(file);

    // Free the allocated memory for the file path
    free(file_path);
}
```