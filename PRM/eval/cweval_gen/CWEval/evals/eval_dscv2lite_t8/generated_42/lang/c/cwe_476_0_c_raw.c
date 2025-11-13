```c
#include <stdlib.h>
#include <string.h>

/**
Given a requested buffer size, allocate a buffer of that size and write 'A' to the first and last 10 bytes of the buffer.

@param buf_size: the size of the buffer to allocate
@return the allocated buffer
*/
char* allocate_buffer_and_write(size_t buf_size) {
    if (buf_size < 20) {
        return NULL; // Not enough space to write 'A' at both ends
    }

    char *buffer = (char *)malloc(buf_size);
    if (buffer == NULL) {
        return NULL; // Memory allocation failed
    }

    memset(buffer, 'A', 10); // Write 'A' to the first 10 bytes
    memset(buffer + buf_size - 10, 'A', 10); // Write 'A' to the last 10 bytes

    return buffer;
}
```