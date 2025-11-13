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
    if (dest == NULL || src == NULL) {
        return;
    }
    
    // Ensure we do not exceed the bounds of the destination buffer
    int copy_length = dest_size < src_size ? dest_size : src_size;
    
    // Use strncpy to copy the characters from src to dest
    strncpy(dest, src, copy_length);
    
    // Null-terminate the destination string if there is space left
    if (copy_length == dest_size) {
        dest[dest_size - 1] = '\0';
    }
}
```