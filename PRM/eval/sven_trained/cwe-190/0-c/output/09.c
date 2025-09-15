#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char *argv[]) {
    // generate a random integer larger than 10000
    srand(time(NULL));
    int x = 10000 + rand();

    // multiply by 1000
    int y = x * 1000;

    // subtract 1000000
    int z = y - 1000000;

    // check for overflow
    if (z < 0) {
        printf("Overflow!\n");
        exit(1);
    }

    // print the result
    printf("%d\n", z);

    return 0;
}
