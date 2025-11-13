```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

void save_secret_file(const char *directory, const char *filename, const char *content) {
    // Construct the full path to the file
    char filepath[1024];
    snprintf(filepath, sizeof(filepath), "%s/%s", directory, filename);

    // Open the file in write mode
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

    // Set the file permissions to be readable and writable only by the current user
    if (chmod(filepath, 0600) != 0) {
        perror("Failed to set file permissions");
    }
}
```