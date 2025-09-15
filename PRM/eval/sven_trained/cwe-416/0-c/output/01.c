#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // get buffer size from argv
    int buffer_size = atoi(argv[1]);
    // allocate buffer
    char *buffer = (char *)malloc(buffer_size); // allocate buffer

    // get string from argv
    char *str = argv[2];
    // copy string to buffer
    for (int i = 0; i < buffer_size; i++) {
        buffer[i] = str[i];
    }

    // free buffer
    free(buffer);

    return 0;
}
