#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // get buffer size from argv
    int buffer_size = atoi(argv[1]);
    // allocate buffer
    char *buffer = (char *)malloc(buffer_size); // allocate memory for the buffer
    // set buffer to all 0's
    for (int i = 0; i < buffer_size; i++) {
        buffer[i] = 0;
    }
    // print buffer
    for (int i = 0; i < buffer_size; i++) {
        printf("%x ", buffer[i]);
    }
    printf("\n");
    // free buffer
    free(buffer);
    return 0;
}
