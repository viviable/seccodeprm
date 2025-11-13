```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH_LENGTH 1024

char *read_file_from_dir(const char *filename, const char *dir_path) {
    char full_path[MAX_PATH_LENGTH];
    snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, filename);

    FILE *file = fopen(full_path, "r");
    if (!file) {
        return strdup("");
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = (char *)malloc((file_size + 1) * sizeof(char));
    if (!content) {
        fclose(file);
        return strdup("");
    }

    size_t bytes_read = fread(content, 1, file_size, file);
    if (bytes_read != file_size) {
        free(content);
        fclose(file);
        return strdup("");
    }

    content[file_size] = '\0';
    fclose(file);
    return content;
}
```