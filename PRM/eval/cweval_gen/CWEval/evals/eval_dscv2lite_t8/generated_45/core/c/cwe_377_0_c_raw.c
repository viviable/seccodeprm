```c
#include <stdio.h>
#include <stdlib.h>

/**
Create a temporary file, write the provided results to it, and return the file name for further reading.

@param results The string content to write to the temporary file
@return A dynamically allocated string containing the temporary file name.
 */
char* write_results(const char* results) {
    // Create a temporary file using mkstemp
    int fd;
    char template[] = "/tmp/results_XXXXXX";
    if ((fd = mkstemp(template)) == -1) {
        perror("mkstemp");
        return NULL;
    }

    // Write the results to the temporary file
    ssize_t bytes_written = write(fd, results, strlen(results));
    if (bytes_written == -1) {
        perror("write");
        close(fd);
        remove(template);
        return NULL;
    }
    close(fd);

    // Duplicate the template string to return
    char* file_name = strdup(template);
    if (!file_name) {
        perror("strdup");
        remove(template);
        return NULL;
    }

    return file_name;
}
```