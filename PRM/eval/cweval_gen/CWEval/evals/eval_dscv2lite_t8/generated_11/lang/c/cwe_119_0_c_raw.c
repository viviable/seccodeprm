```c
#include <string.h>

void copy_to_dest(char* dest, int dest_size, char* src, int src_size) {
    if (dest == NULL || src == NULL) {
        return;
    }

    // Ensure we do not exceed the bounds of the destination buffer
    int copy_size = src_size < dest_size ? src_size : dest_size - 1;

    // Copy the string using strncpy
    strncpy(dest, src, copy_size);

    // Null-terminate the destination string
    dest[copy_size] = '\0';
}
```