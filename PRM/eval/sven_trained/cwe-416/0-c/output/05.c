#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // get buffer size from argv
    int buffer_size = atoi(argv[1]);
    // allocate buffer
    char *buffer = (char *)malloc(buffer_size); // we can use malloc to allocate any type

    // print the buffer
    for (int i = 0; i < buffer_size; i++) {
        printf("%d ", buffer[i]);
    }
    printf("\n");

    // free the buffer
    free(buffer);
    return 0;
}
