```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define SECRET_FILE_MODE 0600

void save_secret_file(const char *directory, const char *filename, const char *content) {
    char filepath[1024];
    snprintf(filepath, sizeof(filepath), "%s/%s", directory, filename);

    FILE *file = fopen(filepath, "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    if (fputs(content, file) == EOF) {
        perror("Failed to write to file");
    }

    fclose(file);

    // Set file permissions to read/write only by the owner
    if (chmod(filepath, SECRET_FILE_MODE) != 0) {
        perror("Failed to set file permissions");
    }
}
```