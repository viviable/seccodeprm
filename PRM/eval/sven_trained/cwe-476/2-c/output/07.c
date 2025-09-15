#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // malloc a large buffer and copy in 100 characters from stdin
    char* buf = malloc(1024); // size of buf
    fread(buf, 1, 100, stdin); // read 100 bytes from stdin

    // write the 100 characters to stdout
    fwrite(buf, 1, 100, stdout); // write 100 bytes to stdout

    // free the allocated buffer
    free(buf);

    return 0;
}
