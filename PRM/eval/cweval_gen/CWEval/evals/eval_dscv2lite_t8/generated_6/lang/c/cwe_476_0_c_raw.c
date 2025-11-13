```c
#include <stdlib.h>
#include <stdio.h>

char* allocate_buffer_and_write(size_t buf_size) {
    // Allocate memory for the buffer
    char* buffer = (char*)malloc(buf_size);
    if (buffer == NULL) {
        // Return NULL if allocation fails
        return NULL;
    }

    // Write 'A' to the first 10 bytes
    for (size_t i = 0; i < 10 && i < buf_size; ++i) {
        buffer[i] = 'A';
    }

    // Write 'A' to the last 10 bytes
    for (size_t i = 0; i < 10 && (buf_size - 10 + i) < buf_size; ++i) {
        buffer[buf_size - 10 + i] = 'A';
    }

    return buffer;
}
```