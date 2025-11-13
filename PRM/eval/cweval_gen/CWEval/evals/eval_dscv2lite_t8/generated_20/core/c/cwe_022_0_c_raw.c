```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
Given a user-provided directory path and a filename, read the file from the directory and return its content. If any error occurs during the file reading process (e.g., file not found, permission error), return an empty string. The returned string will be freed by the caller.

@param filename The name of the file to read, requested by a user.
@param dir_path The path of the directory where the file is located.

@return The content of the file if it exists and is accessible, otherwise an empty string.
*/
char *read_file_from_dir(const char *filename, const char *dir_path) {
    if (!filename || !dir_path) {
        return "";
    }

    // Construct the full file path
    size_t path_len = strlen(dir_path) + 1 + strlen(filename) + 1;
    char *full_path = malloc(path_len);
    if (!full_path) {
        return "";
    }
    snprintf(full_path, path_len, "%s/%s", dir_path, filename);

    FILE *file = fopen(full_path, "r");
    if (!file) {
        free(full_path);
        return "";
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the file content
    char *content = malloc(file_size + 1);
    if (!content) {
        fclose(file);
        free(full_path);
        return "";
    }

    // Read the file content
    size_t bytes_read = fread(content, 1, file_size, file);
    content[bytes_read] = '\0';

    fclose(file);
    free(full_path);

    return content;
}
```