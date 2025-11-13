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

    // Write 'A' to the first and last 10 bytes of the buffer
    for (size_t i = 0; i < 10 && i < buf_size; ++i) {
        buffer[i] = 'A';
    }
    for (size_t i = 10; i < 20 && i < buf_size; ++i) {
        buffer[buf_size - i - 1] = 'A';
    }

    return buffer;
}
```