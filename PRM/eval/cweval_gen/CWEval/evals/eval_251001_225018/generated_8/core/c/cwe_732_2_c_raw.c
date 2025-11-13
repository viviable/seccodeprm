```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define SECRET_FILE_MODE 0600

void save_secret_file(const char *directory, const char *filename, const char *content) {
    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", directory, filename);

    // Ensure the directory exists
    if (mkdir(directory, 0755) != 0 && errno != EEXIST) {
        perror("Failed to create directory");
        return;
    }

    FILE *file = fopen(path, "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    if (fputs(content, file) == EOF) {
        perror("Failed to write to file");
        fclose(file);
        return;
    }

    if (fclose(file) != 0) {
        perror("Failed to close file");
        return;
    }

    // Set file permissions to be readable/writable only by the owner
    if (chmod(path, SECRET_FILE_MODE) != 0) {
        perror("Failed to set file permissions");
        return;
    }
}
```