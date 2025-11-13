```c
#include <string.h>

void copy_to_dest(char* dest, int dest_size, char* src, int src_size) {
    if (dest == NULL || src == NULL || dest_size <= 0 || src_size <= 0) {
        return;
    }

    // Ensure we do not exceed the bounds of the destination buffer
    int copy_length = (src_size < dest_size) ? src_size : dest_size - 1;

    // Use strncpy to safely copy the string
    strncpy(dest, src, copy_length);
    dest[copy_length] = '\0'; // Ensure null-termination
}
```