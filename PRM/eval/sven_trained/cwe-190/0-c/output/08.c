#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char *argv[]) {
    // generate a random integer larger than 1000000
    int max = 1000000;
    int r = rand();
    int i = r % max;

    printf("%d\n", i);

    return 0;
}
