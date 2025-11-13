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

    // Initialize the buffer with 'A'
    memset(buffer, 'A', buf_size);

    // Ensure we don't go out of bounds when writing to the first and last 10 bytes
    size_t num_bytes_to_write = 10;
    if (buf_size >= 2 * num_bytes_to_write) {
        // Write 'A' to the first 10 bytes
        memset(buffer, 'A', num_bytes_to_write);

        // Write 'A' to the last 10 bytes
        memset(buffer + buf_size - num_bytes_to_write, 'A', num_bytes_to_write);
    }

    return buffer;
}
```