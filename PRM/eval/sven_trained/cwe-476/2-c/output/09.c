#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // malloc a large buffer and copy in 100 characters from stdin
    char* buf = malloc(1024); // allocate 1024 bytes
    if (buf == NULL) {
        // malloc failed
        return -1;
    }
    int num_chars = 0;
    while ((num_chars = fgetc(stdin)) != EOF) {
        buf[num_chars] = num_chars;
    }
    // we did not null terminate the string, so this will segfault
    puts(buf);
    free(buf);
    return 0;
}
