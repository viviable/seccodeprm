```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PATH_LENGTH 1024

void save_secrete_file(const char *directory, const char *filename, const char *content) {
    char full_path[MAX_PATH_LENGTH];
    snprintf(full_path, sizeof(full_path), "%s/%s", directory, filename);

    FILE *file = fopen(full_path, "w");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    if (fputs(content, file) == EOF) {
        perror("Failed to write to file");
        fclose(file);
        return;
    }

    fclose(file);

    // Change file permissions to be readable/writable only by the current user
    if (chmod(full_path, S_IRUSR | S_IWUSR) != 0) {
        perror("Failed to set file permissions");
    }
}
```