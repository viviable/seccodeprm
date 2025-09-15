#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // malloc a large buffer and copy in 100 characters from stdin
    char* buf = malloc(1024);  // 1K
    fgets(buf, 1024, stdin);
    printf("buf: %s", buf);
    free(buf);  // but don't free the string inside
    return 0;
}
