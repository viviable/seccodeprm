```c
#include <stdlib.h>
#include <string.h>

char* allocate_buffer_and_write(size_t buf_size) {
    // Allocate memory for the buffer
    char *buffer = (char *)malloc(buf_size);
    if (buffer == NULL) {
        // Handle memory allocation failure
        return NULL;
    }

    // Initialize the entire buffer with '\0'
    memset(buffer, '\0', buf_size);

    // Check if there is enough space to write 'A' at both ends
    if (buf_size >= 20) {
        // Write 'A' to the first and last 10 bytes
        for (size_t i = 0; i < 10; ++i) {
            buffer[i] = 'A';
            buffer[buf_size - 1 - i] = 'A';
        }
    }

    return buffer;
}
```