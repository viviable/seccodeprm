#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // malloc a large buffer and copy in 100 characters from stdin
    char* buf = malloc(1024); // allocate a 1024 byte buffer
    if (buf == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    int n = fread(buf, 1, 100, stdin); // copy 100 bytes from stdin to buf
    if (n < 100) {
        printf("Short read, only %d bytes\n", n);
    }
    printf("Read %d bytes: '%s'\n", n, buf);

    // printf to stdout
    printf("Hello, world!\n");
    return 0;
}
