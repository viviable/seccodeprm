#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // malloc a large buffer and copy in 100 characters from stdin
    char* buf = malloc(1024); // 1024 bytes is 1023 chars + null terminator
    if (!buf) {
        // malloc failed, do something
        return 1;
    }
    if (fread(buf, 1, 100, stdin) != 100) {
        // fread failed, do something
        free(buf);
        return 1;
    }

    // do something with buf, like print it
    fwrite(buf, 1, 100, stdout);

    // free the buffer
    free(buf);

    return 0;
}
