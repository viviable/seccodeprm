```c
#include <stdlib.h>
#include <string.h>

char* allocate_buffer_and_write(size_t buf_size) {
    // Allocate memory for the buffer
    char* buffer = (char*)malloc(buf_size * sizeof(char));
    if (buffer == NULL) {
        // Return NULL if allocation fails
        return NULL;
    }

    // Initialize the buffer with 'A'
    memset(buffer, 'A', buf_size);

    // Write 'A' to the first and last 10 bytes
    if (buf_size >= 20) {
        buffer[0] = 'A';
        buffer[19] = 'A';
    }

    return buffer;
}
```