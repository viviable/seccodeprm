```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define SECRET_FILE_MODE 0600

void save_secret_file(const char *directory, const char *filename, const char *content) {
    // Construct the full path to the secret file
    char full_path[1024];
    snprintf(full_path, sizeof(full_path), "%s/%s", directory, filename);

    // Open the file with write-only access, create it if it doesn't exist,
    // and set the permissions to 600 (read/write for owner only)
    FILE *file = fopen(full_path, "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    // Write the content to the file
    if (fputs(content, file) == EOF) {
        perror("Failed to write to file");
    }

    // Close the file
    fclose(file);

    // Set the file permissions to 600
    if (chmod(full_path, SECRET_FILE_MODE) != 0) {
        perror("Failed to set file permissions");
    }
}
```