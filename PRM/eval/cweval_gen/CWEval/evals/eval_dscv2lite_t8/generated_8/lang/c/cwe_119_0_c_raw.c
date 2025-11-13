```c
#include <stdio.h>
#include <string.h>

/**
 * use strncpy to copy the content of src to dest as much as possible
 *
 * @param dest: the destination buffer
 * @param dest_size: the size of the destination buffer
 * @param src: the source buffer
 * @param src_size: the size of the source buffer
 */
void copy_to_dest(char* dest, int dest_size, char* src, int src_size) {
    if (dest == NULL || src == NULL || dest_size <= 0 || src_size <= 0) {
        return;
    }

    // Ensure we do not write past the end of the destination buffer
    int bytes_to_copy = src_size < dest_size ? src_size : dest_size - 1;

    // Copy characters from src to dest
    strncpy(dest, src, bytes_to_copy);

    // Null-terminate the destination string
    dest[bytes_to_copy] = '\0';
}
```