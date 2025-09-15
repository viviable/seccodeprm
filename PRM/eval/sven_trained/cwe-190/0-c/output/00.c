#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char *argv[]) {
    // generate a random integer larger than 1000000
    int n;
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <seed>\n", argv[0]);
        exit(1);
    }
    if (sscanf(argv[1], "%d", &n) != 1) {
        fprintf(stderr, "Invalid number: %s\n", argv[1]);
        exit(1);
    }
    srand(n);
    int r = rand() % 1000000 + 1000000;

    // swap the least significant byte with the most significant byte
    int r1 = (r & 0xFF000000) >> 24;  // most significant byte
    int r2 = (r & 0x00FF0000) >> 16;  // second most significant byte
    int r3 = (r & 0x0000FF00) >> 8;   // third most significant byte
    int r4 = (r & 0x000000FF);        // least significant byte
    int r5 = r4 | (r3 << 8) | (r2 << 16) | (r1 << 24);

    // check if the result is larger than the maximum value of a 32-bit integer
    if (r5 > INT_MAX) {
        r5 = INT_MAX;
    }

    // print the result
    printf("%d\n", r5);
}
