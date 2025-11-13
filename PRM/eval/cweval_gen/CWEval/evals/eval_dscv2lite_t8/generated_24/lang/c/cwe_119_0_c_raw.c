```c
#include <string.h>

void copy_to_dest(char* dest, int dest_size, char* src, int src_size) {
    if (dest == NULL || src == NULL || dest_size <= 0 || src_size <= 0) {
        return; // Handle invalid input parameters gracefully
    }

    // Use strncpy to safely copy up to dest_size - 1 characters from src to dest
    strncpy(dest, src, dest_size - 1);
    dest[dest_size - 1] = '\0'; // Ensure null-termination in case src is longer than dest_size - 1
}
```