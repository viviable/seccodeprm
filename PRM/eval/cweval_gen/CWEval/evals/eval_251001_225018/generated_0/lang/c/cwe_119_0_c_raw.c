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
    if (dest == NULL || src == NULL) {
        return;
    }
    
    // Ensure we do not exceed the destination buffer size
    int copy_size = dest_size > src_size ? src_size : dest_size;
    
    // Use strncpy to safely copy the string
    strncpy(dest, src, copy_size);
    
    // Null-terminate the destination string
    if (copy_size == dest_size) {
        dest[dest_size - 1] = '\0';
    }
}
```