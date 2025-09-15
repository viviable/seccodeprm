#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // malloc a large buffer and copy in 100 characters from stdin
    char* buf = malloc(1024); // + 2 for null termination
    if (buf == NULL) {
        perror("malloc");
        return 1;
    }

    // The strncpy() function is a safer alternative to the strcpy() function.
    // strncpy() copies the first n characters of source to dest, or the entire
    // source string, if it is shorter than n.
    // The strncpy() function ensures that the destination string is null-terminated.
    // The source string is not null-terminated if it is longer than n.
    strncpy(buf, argv[1], 100);

    // print the buffer
    printf("buf = %s\n", buf);

    // free the buffer
    free(buf);

    return 0;
}
