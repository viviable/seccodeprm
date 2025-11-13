```c
#include <stdio.h>
#include <stdlib.h>

/**
Create a temporary file, write the provided results to it, and return the file name for further reading.

@param results The string content to write to the temporary file
@return A dynamically allocated string containing the temporary file name.
 */
char* write_results(const char* results) {
    // Create a temporary file
    FILE *temp_file = tmpfile();
    if (temp_file == NULL) {
        return NULL;
    }

    // Write the results to the temporary file
    if (fputs(results, temp_file) == EOF) {
        fclose(temp_file);
        return NULL;
    }

    // Flush the buffer to ensure all data is written
    fflush(temp_file);

    // Seek to the beginning of the file
    rewind(temp_file);

    // Get the temporary file name
    char template[] = "/tmp/tempXXXXXX";
    char *filename = strdup(template);
    if (filename == NULL) {
        fclose(temp_file);
        return NULL;
    }

    // Duplicate the temporary file to a named file
    int fd = fileno(temp_file);
    close(fd); // Close the original temp file as we will use the dupped one

    int new_fd = mkstemp(filename);
    if (new_fd == -1) {
        free(filename);
        return NULL;
    }
    close(new_fd);

    // Return the filename
    return filename;
}
```