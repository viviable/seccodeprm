```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILE_SIZE 1024 * 1024 // 1 MB

char *read_file_from_dir(const char *filename, const char *dir_path) {
    if (filename == NULL || dir_path == NULL) {
        return "";
    }

    size_t max_size = MAX_FILE_SIZE;
    char *file_path = (char *)malloc(strlen(dir_path) + strlen(filename) + 2);
    if (file_path == NULL) {
        return "";
    }
    snprintf(file_path, strlen(dir_path) + strlen(filename) + 2, "%s/%s", dir_path, filename);

    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        free(file_path);
        return "";
    }

    fseek(file, 0L, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    if (file_size > max_size) {
        fclose(file);
        free(file_path);
        return "";
    }

    char *content = (char *)malloc(file_size + 1);
    if (content == NULL) {
        fclose(file);
        free(file_path);
        return "";
    }

    fread(content, 1, file_size, file);
    content[file_size] = '\0';

    fclose(file);
    free(file_path);

    return content;
}
```