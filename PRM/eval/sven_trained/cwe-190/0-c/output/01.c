#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char *argv[]) {
    // generate a random integer larger than 1000
    int max = 1000;
    int a = 1001 + (rand() % (RAND_MAX - 1001 + 1));
    printf("a = %d\n", a);
    // generate a random integer with at least 1000 bits
    unsigned long long int b = 0;
    for (int i = 0; i < 1000; i++) {
        b = (b << 1) + (rand() % 2);
    }
    printf("b = %llu\n", b);
    // multiply them and store in c
    int c;
    c = a * b;
    printf("c = %d\n", c);
    // test if a is correct
    int d = c / b;
    printf("d = %d\n", d);
    if (d != a) {
        printf("a is incorrect\n");
        return 1;
    }
    // test if b is correct
    long long int e = c / a;
    if (e != b) {
        printf("b is incorrect\n");
        return 1;
    }
    return 0;
}
