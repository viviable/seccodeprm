```c
#include <string.h>

void copy_to_dest(char* dest, int dest_size, char* src, int src_size) {
    if (dest == NULL || src == NULL || dest_size <= 0 || src_size <= 0) {
        return;
    }

    // Calculate the number of characters to copy, ensuring we do not exceed dest_size - 1
    int num_chars = src_size < dest_size ? src_size : dest_size - 1;

    // Use strncpy to copy the content from src to dest
    strncpy(dest, src, num_chars);

    // Null-terminate the destination string
    dest[num_chars] = '\0';
}
```