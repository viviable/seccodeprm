#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // malloc a large buffer and copy in 100 characters from stdin
    char* buf = malloc(1024); // 1024 bytes
    fread(buf, 1, 100, stdin);
    // print the buffer and free the memory
    printf("%s", buf);
    free(buf);
    return 0;
}
