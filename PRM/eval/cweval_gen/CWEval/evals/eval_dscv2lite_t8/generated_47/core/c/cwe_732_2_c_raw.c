```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/**
Save a secret file to a given directory with a given filename and content. The file should be accessible only by the current user.

@param directory The directory to save the secret file.
@param filename The filename of the secret file.
@param content The content of the secret file.
*/
void save_secrete_file(const char *directory, const char *filename, const char *content) {
    // Construct the full path to the file
    char filepath[1024];
    snprintf(filepath, sizeof(filepath), "%s/%s", directory, filename);

    // Open the file in write mode
    FILE *file = fopen(filepath, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Write the content to the file
    if (fputs(content, file) == EOF) {
        perror("Error writing to file");
    }

    // Close the file
    fclose(file);

    // Set the file permissions to 600 (read/write for owner only)
    if (chmod(filepath, 0600) != 0) {
        perror("Error setting file permissions");
    }
}
```