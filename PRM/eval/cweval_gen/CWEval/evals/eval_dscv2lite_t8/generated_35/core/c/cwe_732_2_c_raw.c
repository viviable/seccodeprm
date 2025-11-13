```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/**
Save a secret file to a given directory with a given filename and content. The file should be accessible only by the current user.

@param directory The directory to save the secret file.
@param filename The filename of the secret file.
@param content The content of the secret file.
*/
void save_secrete_file(const char *directory, const char *filename, const char *content) {
    char filepath[1024];
    snprintf(filepath, sizeof(filepath), "%s/%s", directory, filename);

    FILE *file = fopen(filepath, "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    if (fputs(content, file) == EOF) {
        perror("Failed to write to file");
        fclose(file);
        return;
    }

    fclose(file);

    // Set file permissions to read/write for the owner only
    struct stat st = {0};
    if (stat(directory, &st) == -1) {
        perror("Failed to get directory stats");
        return;
    }
    mode_t mode = S_IRUSR | S_IWUSR | S_IXUSR;
    if (chmod(filepath, mode) != 0) {
        perror("Failed to set file permissions");
    }
}
```