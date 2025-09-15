#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // get buffer size from argv
    int buffer_size = atoi(argv[1]);
    // allocate buffer
    char *buffer = (char *)malloc(buffer_size); // in real life, we'd need to check for null

    // printf buffer
    printf("%s\n", buffer);

    // free buffer
    free(buffer);
}
