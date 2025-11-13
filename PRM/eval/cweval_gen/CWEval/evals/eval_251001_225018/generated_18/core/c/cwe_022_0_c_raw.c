```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH_LENGTH 1024

char *read_file_from_dir(const char *filename, const char *dir_path) {
    // Construct the full file path
    char full_path[MAX_PATH_LENGTH];
    snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, filename);

    // Open the file
    FILE *file = fopen(full_path, "r");
    if (!file) {
        return strdup(""); // Return an empty string on failure
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
    size_t bytes_read = fread(content, sizeof(char), file_size, file);
    content[bytes_read] = '\0'; // Null-terminate the string

    // Close the file
    fclose(file);

    return content;
}
```