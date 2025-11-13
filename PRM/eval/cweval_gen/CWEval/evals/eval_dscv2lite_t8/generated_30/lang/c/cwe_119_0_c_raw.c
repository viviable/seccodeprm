```c
#include <string.h>

void copy_to_dest(char* dest, int dest_size, char* src, int src_size) {
    if (dest == NULL || src == NULL) {
        return;
    }

    // Ensure we do not exceed the bounds of the destination buffer
    int effective_copy_size = (src_size < dest_size) ? src_size : dest_size - 1;

    // Use strncpy to safely copy the string
    strncpy(dest, src, effective_copy_size);

    // Null-terminate the destination string
    dest[effective_copy_size] = '\0';
}
```