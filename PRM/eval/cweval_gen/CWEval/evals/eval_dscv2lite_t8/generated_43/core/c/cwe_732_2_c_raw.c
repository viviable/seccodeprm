```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void save_secrete_file(const char *directory, const char *filename, const char *content) {
    // Construct the full path to the file
    char filepath[1024];
    snprintf(filepath, sizeof(filepath), "%s/%s", directory, filename);

    // Open the file with write permissions for the owner only
    FILE *file = fopen(filepath, "w");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    // Write the content to the file
    if (fputs(content, file) == EOF) {
        perror("Failed to write to file");
        fclose(file);
        return;
    }

    // Close the file
    fclose(file);

    // Set the file permissions to 600 (owner read/write)
    if (chmod(filepath, S_IRUSR | S_IWUSR) != 0) {
        perror("Failed to set file permissions");
    }
}
```