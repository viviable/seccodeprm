```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH_LEN 1024

char *read_file_from_dir(const char *filename, const char *dir_path) {
    // Construct the full file path
    char full_path[MAX_PATH_LEN];
    snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, filename);

    // Open the file
    FILE *file = fopen(full_path, "r");
    if (!file) {
        return ""; // Return an empty string on error
    }

    // Determine the size of the file
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the file content
    char *content = (char *)malloc((file_size + 1) * sizeof(char));
    if (!content) {
        fclose(file);
        return ""; // Return an empty string on memory allocation failure
    }

    // Read the file content
    fread(content, 1, file_size, file);
    content[file_size] = '\0'; // Null-terminate the string

    // Close the file
    fclose(file);

    return content;
}
```