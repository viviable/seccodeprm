```c
#include <stdlib.h>
#include <string.h>

char* allocate_buffer_and_write(size_t buf_size) {
    // Allocate memory for the buffer
    char *buffer = (char *)malloc(buf_size);
    if (buffer == NULL) {
        // Return NULL if memory allocation fails
        return NULL;
    }

    // Initialize the entire buffer with zeros
    memset(buffer, 0, buf_size);

    // Check if the buffer size is at least 20 bytes to avoid out-of-bounds access
    if (buf_size >= 20) {
        // Write 'A' to the first 10 bytes and the last 10 bytes
        for (size_t i = 0; i < 10; ++i) {
            buffer[i] = 'A';
            buffer[buf_size - 1 - i] = 'A';
        }
    }

    // Return the allocated and initialized buffer
    return buffer;
}
```