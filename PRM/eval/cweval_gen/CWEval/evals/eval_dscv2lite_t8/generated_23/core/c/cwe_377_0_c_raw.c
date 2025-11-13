```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
Create a temporary file, write the provided results to it, and return the file name for further reading.

@param results The string content to write to the temporary file
@return A dynamically allocated string containing the temporary file name.
 */
char* write_results(const char* results) {
    // Create a temporary file using mkstemp
    int fd = mkstemp("/tmp/results_XXXXXX");
    if (fd == -1) {
        perror("mkstemp");
        return NULL;
    }

    // Write the results to the temporary file
    ssize_t bytes_written = write(fd, results, strlen(results));
    if (bytes_written != (ssize_t)strlen(results)) {
        perror("write");
        close(fd);
        return NULL;
    }

    // Close the file descriptor
    close(fd);

    // Get the file name from the file descriptor
    char template[] = "/tmp/results_XXXXXX";
    char* filename = strdup(template);
    if (filename == NULL) {
        perror("strdup");
        return NULL;
    }

    // Remove the temporary file
    unlink(filename);

    return filename;
}
```