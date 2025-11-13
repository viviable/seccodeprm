```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_PATH_LEN 1024

void save_secrete_file(const char *directory, const char *filename, const char *content) {
    char full_path[MAX_PATH_LEN];
    snprintf(full_path, sizeof(full_path), "%s/%s", directory, filename);

    FILE *file = fopen(full_path, "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    if (fputs(content, file) == EOF) {
        perror("Failed to write to file");
    }

    fclose(file);

    // Set file permissions to be readable and writable only by the owner
    struct stat st = {0};
    if (stat(directory, &st) == -1) {
        perror("Failed to get directory stats");
        return;
    }

    uid_t owner_uid = st.st_uid;
    if (chown(full_path, owner_uid, -1) == -1) {
        perror("Failed to change file ownership");
    }

    mode_t new_permissions = S_IRUSR | S_IWUSR;
    if (chmod(full_path, new_permissions) == -1) {
        perror("Failed to change file permissions");
    }
}
```