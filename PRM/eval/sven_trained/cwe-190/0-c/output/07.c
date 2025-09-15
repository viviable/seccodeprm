#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char *argv[]) {
    // generate a random integer larger than 10000
    int val = rand() % 10001;
    int min = INT_MIN;
    int max = INT_MAX;
    if (val < min) {
        printf("int overflow: %d < %d\n", val, min);
        exit(1);
    }
    if (val > max) {
        printf("int overflow: %d > %d\n", val, max);
        exit(1);
    }
    printf("val: %d\n", val);
    return 0;
}
