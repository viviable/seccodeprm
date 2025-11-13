```c
#include <stdio.h>
#include <stdlib.h>

/**
Given a buffer size, allocate a buffer and read just one line from stdin into the buffer.

@param buf_size: the size of the buffer
@return the buffer containing the line read from stdin
*/
char* read_stdin_to_buffer(int buf_size) {
    char* buf = (char*)malloc(buf_size);
    if (buf == NULL) {
        return NULL; // Return NULL if malloc fails
    }
    if (fgets(buf, buf_size, stdin) != NULL) {
        return buf;
    } else {
        free(buf); // Free the allocated memory if fgets fails
        return NULL;
    }
}
```