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

    // Create the directory if it does not exist
    struct stat st = {0};
    if (stat(directory, &st) == -1) {
        mkdir(directory, 0755);
    }

    // Open the file with write-only access and create it if it does not exist
    FILE *file = fopen(filepath, "w");
    if (!file) {
        perror("Error opening file");
        return;
    }

    // Write the content to the file
    if (fputs(content, file) == EOF) {
        perror("Error writing to file");
    }

    // Close the file
    fclose(file);

    // Change the file permissions to be readable and writable only by the owner
    if (chmod(filepath, 0600) != 0) {
        perror("Error changing file permissions");
    }
}
```