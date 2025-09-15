#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char *argv[]) {
    // generate a random integer larger than 100000
    int num;
    int base = 100000;
    int digits = 5;
    for (int i = 0; i < digits; i++) {
        num *= 10;
        num += (rand() % 10);
    }
    // cast to unsigned int to avoid overflow
    unsigned int num2 = num + base;

    // use function to print the number
    print_number(num2);

    // print the number
    printf("%u\n", num2);

    return 0;
}
