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
    char* buffer = (char*)malloc(buf_size);
    if (buffer == NULL) {
        // Handle memory allocation failure
        return NULL;
    }

    // Write 'A' to the first 10 bytes
    memset(buffer, 'A', 10);

    // If there is enough space, write 'A' to the last 10 bytes
    if (buf_size > 20) {
        memset(&buffer[buf_size - 10], 'A', 10);
    }

    return buffer;
}
```