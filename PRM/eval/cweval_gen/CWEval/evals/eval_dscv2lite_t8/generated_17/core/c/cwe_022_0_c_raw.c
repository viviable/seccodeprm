```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILE_SIZE 1024 * 1024 // 1MB

char *read_file_from_dir(const char *filename, const char *dir_path) {
    char full_path[MAX_FILE_SIZE];
    snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, filename);

    FILE *file = fopen(full_path, "r");
    if (!file) {
        return "";
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size > MAX_FILE_SIZE - 1) {
        fclose(file);
        return "";
    }

    char *content = (char *)malloc((file_size + 1) * sizeof(char));
    if (!content) {
        fclose(file);
        return "";
    }

    size_t bytes_read = fread(content, 1, file_size, file);
    if (bytes_read != file_size) {
        free(content);
        fclose(file);
        return "";
    }

    content[file_size] = '\0';
    fclose(file);
    return content;
}
```