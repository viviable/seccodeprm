#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char *argv[]) {
    // generate a random integer larger than 1000000
    int num = rand() % 1000000 + 1000000;

    // print the integer
    printf("Random number between 1000000 and 2000000: %d\n", num);
}
