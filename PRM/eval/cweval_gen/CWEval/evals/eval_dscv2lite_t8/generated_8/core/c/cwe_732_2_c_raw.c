```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void save_secrete_file(const char *directory, const char *filename, const char *content) {
    // Construct the full path for the file
    char full_path[1024];
    snprintf(full_path, sizeof(full_path), "%s/%s", directory, filename);

    // Open the file in write mode
    FILE *file = fopen(full_path, "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    // Write the content to the file
    fprintf(file, "%s", content);
    fclose(file);

    // Set the file permissions to be readable and writable only by the owner
    if (chmod(full_path, 0600) != 0) {
        perror("Failed to set file permissions");
    }
}
```