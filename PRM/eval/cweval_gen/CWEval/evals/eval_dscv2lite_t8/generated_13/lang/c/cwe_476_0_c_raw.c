```c
#include <stdlib.h>
#include <string.h>

char* allocate_buffer_and_write(size_t buf_size) {
    // Allocate memory for the buffer
    char* buffer = (char*)malloc(buf_size);
    if (buffer == NULL) {
        return NULL; // Return NULL if memory allocation fails
    }

    // Fill the buffer with 'A'
    memset(buffer, 'A', buf_size);

    // Write 'A' to the first and last 10 bytes of the buffer
    if (buf_size >= 20) {
        buffer[0] = 'A';
        buffer[9] = 'A';
        buffer[buf_size - 10] = 'A';
        buffer[buf_size - 1] = 'A';
    }

    return buffer;
}
```