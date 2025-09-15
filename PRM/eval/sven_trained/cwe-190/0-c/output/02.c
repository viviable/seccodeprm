#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char *argv[]) {
    // generate a random integer larger than 10000
    int r = 10000 + rand() % 10000;
    // make a pointer to the integer
    int *p = &r;
    // print the address and the value
    printf("Address: %p, Value: %d\n", (void*)p, *p);
    // return 0
    return 0;
}
