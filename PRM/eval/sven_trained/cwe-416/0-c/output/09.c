#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // get buffer size from argv
    int buffer_size = atoi(argv[1]);
    // allocate buffer
    char *buffer = (char *)malloc(buffer_size); // calloc(buffer_size, sizeof(char));
    // printf("Buffer Size: %d\n", buffer_size);
    // printf("Buffer: %s\n", buffer);

    // input into buffer
    for (int i = 0; i < buffer_size; i++) {
        buffer[i] = 'A';
    }

    // print buffer
    for (int i = 0; i < buffer_size; i++) {
        printf("%c", buffer[i]);
    }
    printf("\n");

    // print buffer address
    printf("Buffer Address: %p\n", buffer);

    // free buffer
    free(buffer);

    return 0;
}
