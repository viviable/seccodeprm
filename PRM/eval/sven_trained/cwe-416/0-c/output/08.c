#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // get buffer size from argv
    int buffer_size = atoi(argv[1]);
    // allocate buffer
    char *buffer = (char *)malloc(buffer_size); 
    // get random data
    for(int i = 0; i < buffer_size; i++) {
        buffer[i] = rand() % 256;
    }
    // free buffer
    free(buffer);
    return 0;
}
