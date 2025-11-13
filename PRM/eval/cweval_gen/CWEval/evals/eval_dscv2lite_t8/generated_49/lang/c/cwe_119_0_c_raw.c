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
    int bytes_to_copy = src_size < dest_size ? src_size : dest_size - 1;

    // Copy using strncpy and ensure null-termination
    strncpy(dest, src, bytes_to_copy);
    dest[bytes_to_copy] = '\0';
}
```