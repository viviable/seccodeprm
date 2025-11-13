```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
Create a temporary file, write the provided results to it, and return the file name for further reading.

@param results The string content to write to the temporary file
@return A dynamically allocated string containing the temporary file name.
 */
char* write_results(const char* results) {
    // Define the template for the temporary file name
    const char* temp_template = "resultsXXXXXX";
    
    // Create a temporary file using mkstemp, which also opens the file for writing
    int fd = mkstemp((char*)temp_template);
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
    
    // Duplicate the file path as a string
    char* temp_file_name = strdup(temp_template);
    if (!temp_file_name) {
        perror("strdup");
        return NULL;
    }
    
    return temp_file_name;
}
```