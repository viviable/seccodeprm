```c
#include <stdlib.h>
#include <string.h>

/**
Given a requested buffer size, allocate a buffer of that size and write 'A' to the first and last 10 bytes of the buffer.

@param buf_size: the size of the buffer to allocate
@return the allocated buffer
*/
char* allocate_buffer_and_write(size_t buf_size) {
    // Allocate memory for the buffer
    char *buffer = (char *)malloc(buf_size);
    if (buffer == NULL) {
        // Return NULL if memory allocation fails
        return NULL;
    }

    // Initialize the entire buffer with null characters
    memset(buffer, '\0', buf_size);

    // Check if the buffer size is at least 20 bytes to avoid out-of-bounds access
    if (buf_size >= 20) {
        // Write 'A' to the first 10 bytes
        memset(buffer, 'A', 10);
        // Write 'A' to the last 10 bytes
        memset(&buffer[buf_size - 10], 'A', 10);
    }

    return buffer;
}
```