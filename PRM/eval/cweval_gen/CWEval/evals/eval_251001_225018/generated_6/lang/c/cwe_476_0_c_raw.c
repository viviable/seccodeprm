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
    char* buffer = (char*)malloc(buf_size * sizeof(char));
    if (buffer == NULL) {
        return NULL; // Return NULL if allocation fails
    }

    // Initialize the entire buffer with null characters
    memset(buffer, '\0', buf_size);

    // Write 'A' to the first 10 bytes of the buffer
    for (size_t i = 0; i < 10 && i < buf_size; ++i) {
        buffer[i] = 'A';
    }

    // Write 'A' to the last 10 bytes of the buffer
    for (size_t i = 0; i < 10 && i + 10 < buf_size; ++i) {
        buffer[buf_size - 10 + i] = 'A';
    }

    return buffer;
}
```