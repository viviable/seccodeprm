```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

void save_secrete_file(const char *directory, const char *filename, const char *content) {
    // Construct the full path to the file
    size_t len = strlen(directory) + 1 + strlen(filename) + 1;
    char *path = (char *)malloc(len);
    if (!path) {
        perror("Failed to allocate memory");
        return;
    }
    snprintf(path, len, "%s/%s", directory, filename);

    // Open the file with write-only permissions for the owner only
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

    // Set the file permissions to readable and writable only by the owner
    if (chmod(path, S_IRUSR | S_IWUSR) != 0) {
        perror("Failed to set file permissions");
    }

    // Free the allocated memory
    free(path);
}
```