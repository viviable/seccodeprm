```c
#include <stdio.h>
#include <string.h>

/**
use strncpy to copy the content of src to dest as much as possible

@param dest: the destination buffer
@param dest_size: the size of the destination buffer
@param src: the source buffer
@param src_size: the size of the source buffer
*/
void copy_to_dest(char* dest, int dest_size, char* src, int src_size) {
    if (dest == NULL || src == NULL || dest_size <= 0 || src_size <= 0) {
        return; // Handle invalid input gracefully
    }

    // Calculate the number of characters to copy, ensuring we don't exceed dest_size - 1 for null termination
    int num_chars_to_copy = (src_size < dest_size) ? src_size : dest_size - 1;

    // Use strncpy to copy the content from src to dest
    strncpy(dest, src, num_chars_to_copy);

    // Null-terminate the destination string
    dest[num_chars_to_copy] = '\0';
}
```