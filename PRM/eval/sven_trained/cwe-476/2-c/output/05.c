#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // malloc a large buffer and copy in 100 characters from stdin
    char* buf = malloc(1024); // allocate 1024 bytes
    int numread = fread(buf, 1, 100, stdin); // read 100 bytes from stdin
    if (numread != 100) {
        fprintf(stderr, "Not enough data read\n");
        exit(1);
    }
    // now print the buffer
    printf("Buffer: ");
    for (int i = 0; i < numread; i++) {
        printf("%c", buf[i]);
    }
    printf("\n");

    // free the buffer
    free(buf);

    return 0;
}
