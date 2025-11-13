```c
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
    
    // Ensure we do not exceed the bounds of the destination buffer
    int copy_length = (src_size < dest_size) ? src_size : dest_size - 1;
    
    // Copy the string from src to dest using strncpy
    strncpy(dest, src, copy_length);
    
    // Null-terminate the destination string
    dest[copy_length] = '\0';
}
```