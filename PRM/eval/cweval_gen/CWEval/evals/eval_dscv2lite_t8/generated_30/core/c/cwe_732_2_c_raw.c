```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define SECRET_FILE_PERMISSIONS 0600

void save_secrete_file(const char *directory, const char *filename, const char *content) {
    char full_path[1024];
    snprintf(full_path, sizeof(full_path), "%s/%s", directory, filename);

    FILE *file = fopen(full_path, "w");
    if (file == NULL) {
        perror("Failed to open file for writing");
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

    if (chmod(full_path, SECRET_FILE_PERMISSIONS) != 0) {
        perror("Failed to set file permissions");
        return;
    }
}
```