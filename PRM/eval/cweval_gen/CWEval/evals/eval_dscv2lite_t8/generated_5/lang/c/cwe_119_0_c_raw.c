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
    if (dest == NULL || dest_size <= 0 || src == NULL || src_size <= 0) {
        return; // Handle invalid input parameters
    }

    // Calculate the number of characters to copy
    int num_chars_to_copy = src_size < dest_size ? src_size : dest_size - 1;

    // Use strncpy to copy the content
    strncpy(dest, src, num_chars_to_copy);
    dest[num_chars_to_copy] = '\0'; // Ensure null-termination
}
```