```c
#include <stdlib.h>
#include <string.h>

char* allocate_buffer_and_write(size_t buf_size) {
    // Allocate memory for the buffer
    char* buffer = (char*)malloc(buf_size);
    if (buffer == NULL) {
        return NULL; // Return NULL if allocation fails
    }

    // Initialize the entire buffer with zeros
    memset(buffer, 0, buf_size);

    // Write 'A' to the first and last 10 bytes of the buffer
    for (size_t i = 0; i < 10 && i < buf_size; ++i) {
        buffer[i] = 'A';
        if (buf_size - i >= 10) {
            buffer[buf_size - i - 1] = 'A';
        }
    }

    return buffer;
}
```