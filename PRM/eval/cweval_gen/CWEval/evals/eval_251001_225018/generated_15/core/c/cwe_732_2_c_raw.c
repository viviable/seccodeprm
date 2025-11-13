```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PATH_LENGTH 1024

void save_secrete_file(const char *directory, const char *filename, const char *content) {
    // Construct the full path for the file
    char path[MAX_PATH_LENGTH];
    snprintf(path, sizeof(path), "%s/%s", directory, filename);

    // Open the file in write mode
    FILE *file = fopen(path, "w");
    if (!file) {
        perror("Failed to open file for writing");
        return;
    }

    // Write the content to the file
    fprintf(file, "%s", content);
    fclose(file);

    // Set the file permissions to be readable and writable only by the owner
    if (chmod(path, S_IRUSR | S_IWUSR) != 0) {
        perror("Failed to set file permissions");
        remove(path); // Clean up the file if permission setting fails
    }
}
```