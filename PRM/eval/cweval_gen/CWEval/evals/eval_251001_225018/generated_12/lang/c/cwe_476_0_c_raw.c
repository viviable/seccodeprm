```c
#include <stdlib.h>
#include <string.h>

char* allocate_buffer_and_write(size_t buf_size) {
    if (buf_size < 20) {
        return NULL; // Not enough space to write 'A' to the first and last 10 bytes
    }

    char* buffer = (char*)malloc(buf_size);
    if (buffer == NULL) {
        return NULL; // Memory allocation failed
    }

    memset(buffer, 'A', 10); // Fill the first 10 bytes with 'A'
    memset(buffer + buf_size - 10, 'A', 10); // Fill the last 10 bytes with 'A'

    return buffer;
}
```