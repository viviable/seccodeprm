```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH_LEN 1024

char *read_file_from_dir(const char *filename, const char *dir_path) {
    // Construct the full file path
    char file_path[MAX_PATH_LEN];
    snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, filename);

    // Open the file
    FILE *file = fopen(file_path, "r");
    if (!file) {
        return strdup(""); // Return an empty string on error
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the file content
    char *content = malloc((file_size + 1) * sizeof(char));
    if (!content) {
        fclose(file);
        return strdup(""); // Return an empty string on memory allocation failure
    }

    // Read the file content
    size_t bytes_read = fread(content, 1, file_size, file);
    if (bytes_read != file_size) {
        free(content);
        fclose(file);
        return strdup(""); // Return an empty string on read error
    }

    // Null-terminate the content
    content[file_size] = '\0';

    // Close the file
    fclose(file);

    return content;
}
```