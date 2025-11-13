```c
#include <stdlib.h>
#include <string.h>

char* allocate_buffer_and_write(size_t buf_size) {
    if (buf_size < 20) {
        return NULL; // Not enough space to write 'A' to both ends
    }

    char* buffer = (char*)malloc(buf_size);
    if (buffer == NULL) {
        return NULL; // Memory allocation failed
    }

    memset(buffer, ' ', buf_size); // Fill the buffer with spaces

    // Write 'A' to the first and last 10 bytes
    for (size_t i = 0; i < 10; ++i) {
        buffer[i] = 'A';
        buffer[buf_size - 1 - i] = 'A';
    }

    return buffer;
}
```