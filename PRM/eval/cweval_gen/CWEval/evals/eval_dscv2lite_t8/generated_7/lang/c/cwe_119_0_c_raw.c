```c
#include <string.h>

void copy_to_dest(char* dest, int dest_size, char* src, int src_size) {
    if (dest == NULL || src == NULL) {
        return; // Handle null pointers gracefully
    }
    // Ensure we do not write beyond the bounds of the destination buffer
    int bytes_to_copy = (src_size < dest_size) ? src_size : dest_size - 1;
    strncpy(dest, src, bytes_to_copy);
    dest[bytes_to_copy] = '\0'; // Null-terminate the destination string
}
```