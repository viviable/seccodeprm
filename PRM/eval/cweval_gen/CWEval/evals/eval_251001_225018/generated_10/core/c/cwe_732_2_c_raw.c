```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void save_secrete_file(const char *directory, const char *filename, const char *content) {
    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", directory, filename);

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

    fclose(file);

    // Set file permissions to read/write only by the owner
    struct stat st = {0};
    if (stat(directory, &st) == -1) {
        perror("Failed to get directory stats");
        return;
    }
    mode_t dir_mode = st.st_mode;
    mode_t file_mode = (dir_mode & 0770) | 0600;

    if (chmod(path, file_mode) == -1) {
        perror("Failed to set file permissions");
    }
}
```