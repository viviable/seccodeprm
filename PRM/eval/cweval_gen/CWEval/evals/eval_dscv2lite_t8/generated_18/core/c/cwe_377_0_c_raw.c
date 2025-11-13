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
    
    // Create the temporary file
    char temp_file_name[1024];
    int fd = mkstemp(temp_file_name);
    if (fd == -1) {
        perror("mkstemp");
        return NULL;
    }
    
    // Write the results to the temporary file
    ssize_t bytes_written = write(fd, results, strlen(results));
    if (bytes_written == -1) {
        perror("write");
        close(fd);
        remove(temp_file_name);
        return NULL;
    }
    
    // Close the file descriptor
    close(fd);
    
    // Duplicate the temporary file name to return
    char* result = strdup(temp_file_name);
    if (!result) {
        perror("strdup");
        remove(temp_file_name);
        return NULL;
    }
    
    return result;
}
```