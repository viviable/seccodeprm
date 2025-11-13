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
    if (bytes_written == -1) {
        perror("write");
        close(fd);
        unlink(temp_name); // Clean up the temporary file on error
        return NULL;
    }

    // Close the file descriptor
    close(fd);

    // Get the temporary file name from the file descriptor
    char temp_name[256];
    if (fgetpos(fd, &pos) != 0 || fgetnamebyfd(fd, temp_name, sizeof(temp_name)) == -1) {
        perror("fgetpos or fgetnamebyfd");
        unlink(temp_name); // Clean up the temporary file on error
        return NULL;
    }

    // Duplicate the file name so we can return it
    char* result = strdup(temp_name);
    if (result == NULL) {
        perror("strdup");
        unlink(temp_name); // Clean up the temporary file on error
        return NULL;
    }

    // Unlink the temporary file so it is deleted when closed
    unlink(temp_name);

    return result;
}
```