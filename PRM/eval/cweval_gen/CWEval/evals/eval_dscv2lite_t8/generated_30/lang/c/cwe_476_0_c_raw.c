```c
#include <stdlib.h>
#include <string.h>

char* allocate_buffer_and_write(size_t buf_size) {
    // Allocate memory for the buffer
    char* buffer = (char*)malloc(buf_size);
    if (buffer == NULL) {
        // Return NULL if allocation fails
        return NULL;
    }

    // Initialize the entire buffer with null terminator
    memset(buffer, '\0', buf_size);

    // Check if there is enough space to write 'A' to the first and last 10 bytes
    if (buf_size >= 20) {
        // Write 'A' to the first 10 bytes
        memset(buffer, 'A', 10);
        // Write 'A' to the last 10 bytes
        memset(&buffer[buf_size - 10], 'A', 10);
    }

    return buffer;
}
```