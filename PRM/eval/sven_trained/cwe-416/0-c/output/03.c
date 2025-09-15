#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // get buffer size from argv
    int buffer_size = atoi(argv[1]);
    // allocate buffer
    char *buffer = (char *)malloc(buffer_size); // malloc returns a void pointer, cast to char
    // set buffer to "hello"
    strncpy(buffer, "hello", buffer_size);
    // free buffer
    free(buffer);

    return 0;
}
