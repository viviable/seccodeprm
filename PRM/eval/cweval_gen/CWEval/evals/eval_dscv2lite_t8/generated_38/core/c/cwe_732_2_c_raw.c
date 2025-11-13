```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
Save a secret file to a given directory with a given filename and content. The file should be accessible only by the current user.

@param directory The directory to save the secret file.
@param filename The filename of the secret file.
@param content The content of the secret file.
*/
void save_secret_file(const char *directory, const char *filename, const char *content) {
    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", directory, filename);

    FILE *file = fopen(path, "w");
    if (!file) {
        perror("Failed to open file for writing");
        return;
    }

    if (fputs(content, file) == EOF) {
        perror("Failed to write to file");
    }

    fclose(file);

    // Set file permissions to be readable and writable only by the owner
    if (chmod(path, S_IRUSR | S_IWUSR) != 0) {
        perror("Failed to set file permissions");
    }
}
```