#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char *argv[]) {
    // generate a random integer larger than 1000000
    int n = 1000000 + rand();
    int *x = (int *) malloc(sizeof(int) * (n + 1)); // array of size n+1

    // fill array with random values
    for (int i = 1; i <= n; i++) {
        x[i] = rand();
    }

    // find the minimum value
    int min = INT_MAX;
    for (int i = 1; i <= n; i++) {
        if (x[i] < min) {
            min = x[i];
        }
    }

    // print the minimum value
    printf("The minimum value is %d\n", min);

    // free the memory
    free(x);

    return 0;
}
