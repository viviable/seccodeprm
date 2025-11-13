```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PATH_LENGTH 1024

void save_secrete_file(const char *directory, const char *filename, const char *content) {
    char full_path[MAX_PATH_LENGTH];
    snprintf(full_path, sizeof(full_path), "%s/%s", directory, filename);

    // Create the directory if it doesn't exist
    mkdir(directory, 0700);

    FILE *file = fopen(full_path, "w");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    fprintf(file, "%s", content);
    fclose(file);

    // Set the file permissions to read and write only by the owner
    chmod(full_path, 0600);
}
```