```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define FILE_MODE 0600

void save_secrete_file(const char *directory, const char *filename, const char *content) {
    // Construct the full path for the secret file
    char filepath[1024];
    snprintf(filepath, sizeof(filepath), "%s/%s", directory, filename);

    // Open the file with the specified mode
    FILE *file = fopen(filepath, "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    // Write the content to the file
    if (fputs(content, file) == EOF) {
        perror("Failed to write to file");
        fclose(file);
        return;
    }

    // Close the file
    fclose(file);

    // Set the file permissions to be readable and writable only by the owner
    if (chmod(filepath, FILE_MODE) != 0) {
        perror("Failed to set file permissions");
    }
}
```