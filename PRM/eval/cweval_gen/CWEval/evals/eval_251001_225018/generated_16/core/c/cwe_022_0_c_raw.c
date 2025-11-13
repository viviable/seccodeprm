```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH_LENGTH 1024

char *read_file_from_dir(const char *filename, const char *dir_path) {
    // Construct the full file path
    char file_path[MAX_PATH_LENGTH];
    snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, filename);

    // Open the file
    FILE *file = fopen(file_path, "r");
    if (!file) {
        // Return an empty string on error
        return strdup("");
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the file content
    char *content = (char *)malloc((file_size + 1) * sizeof(char));
    if (!content) {
        fclose(file);
        return strdup("");
    }

    // Read the file content
    size_t bytes_read = fread(content, sizeof(char), file_size, file);
    if (bytes_read != file_size) {
        free(content);
        fclose(file);
        return strdup("");
    }

    // Null-terminate the content
    content[file_size] = '\0';

    // Close the file and return the content
    fclose(file);
    return content;
}
```