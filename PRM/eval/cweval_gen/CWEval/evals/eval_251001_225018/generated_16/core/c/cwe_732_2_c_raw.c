```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SECRET_FILE_MODE 0600

void save_secret_file(const char *directory, const char *filename, const char *content) {
    // Construct the full path to the secret file
    char filepath[1024];
    snprintf(filepath, sizeof(filepath), "%s/%s", directory, filename);

    // Open the file with write-only mode and set permissions to 600 (read/write for owner only)
    FILE *file = fopen(filepath, "w");
    if (file == NULL) {
        perror("Failed to open file for writing");
        return;
    }

    // Set the file permissions to 600
    if (chmod(filepath, SECRET_FILE_MODE) != 0) {
        perror("Failed to set file permissions");
        fclose(file);
        return;
    }

    // Write the content to the file
    if (fputs(content, file) == EOF) {
        perror("Failed to write to file");
    }

    // Close the file
    fclose(file);
}
```