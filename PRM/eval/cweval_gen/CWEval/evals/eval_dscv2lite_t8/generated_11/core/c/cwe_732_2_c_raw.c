```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
    if (!file) {
        perror("Failed to open file");
        return;
    }

    // Write the content to the file
    fprintf(file, "%s", content);
    fclose(file);

    // Change the file permissions to be readable and writable only by the current user
    if (chmod(filepath, 0600) != 0) {
        perror("Failed to set file permissions");
        remove(filepath); // Clean up the file if permission setting fails
    }
}
```