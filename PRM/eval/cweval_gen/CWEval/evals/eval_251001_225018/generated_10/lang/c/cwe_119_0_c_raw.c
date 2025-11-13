```c
#include <string.h>

void copy_to_dest(char* dest, int dest_size, char* src, int src_size) {
    if (dest == NULL || src == NULL || dest_size <= 0 || src_size <= 0) {
        return; // Handle invalid input gracefully
    }
    strncpy(dest, src, dest_size - 1); // Copy up to dest_size-1 characters to leave space for null terminator
    dest[dest_size - 1] = '\0'; // Ensure null termination
}
```