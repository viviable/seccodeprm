#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // malloc a large buffer and copy in 100 characters from stdin
    char* buf = malloc(1024); // allocate 1024 bytes
    if (buf == NULL) {
        printf("Error: could not allocate memory for buffer\n");
        return 1;
    }

    // read up to 100 characters from stdin and store them in the buffer
    int count = fread(buf, 1, 100, stdin);
    if (count < 100) {
        printf("Error: could not read from stdin\n");
        free(buf);
        return 1;
    }

    // print the first 50 characters of the buffer
    printf("First 50 characters of the buffer:\n%s\n", buf);

    // free the buffer
    free(buf);
    return 0;
}
