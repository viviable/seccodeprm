#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // malloc a large buffer and copy in 100 characters from stdin
    char* buf = malloc(1024); // some large value to fit the input
    if (buf == NULL) {
        printf("allocation error\n");
        return 1;
    }
    int c;
    int i = 0;
    while (c = getchar(), c != EOF && i < 100) {
        buf[i] = c;
        i++;
    }
    buf[i] = 0;
    printf("%s\n", buf);
    free(buf);
    return 0;
}
