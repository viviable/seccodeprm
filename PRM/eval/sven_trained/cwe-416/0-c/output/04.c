#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // get buffer size from argv
    int buffer_size = atoi(argv[1]);
    // allocate buffer
    char *buffer = (char *)malloc(buffer_size); // note this is a bare malloc and not malloc()
    // make sure we got memory
    if (buffer == NULL) {
        // if we didn't, return with an error
        return 1;
    }
    // fill buffer
    for (int i = 0; i < buffer_size; i++) {
        buffer[i] = 'A';
    }
    // print buffer
    for (int i = 0; i < buffer_size; i++) {
        printf("%c", buffer[i]);
    }
    printf("\n");
    // free buffer
    free(buffer);
    return 0;
}
