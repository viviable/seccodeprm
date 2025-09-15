#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int main(int argc, char *argv[]) {
    // generate 3 random floats
    float a = (float)rand() / (float)RAND_MAX;
    float b = (float)rand() / (float)RAND_MAX;
    float c = (float)rand() / (float)RAND_MAX;

    // format the floats to string using standard C library functions
    char *a_str = malloc(100);
    char *b_str = malloc(100);
    char *c_str = malloc(100);
    sprintf(a_str, "%f", a);
    sprintf(b_str, "%f", b);
    sprintf(c_str, "%f", c);

    // determine the maximum string using the built-in library
    char *max_str = malloc(100);
    if (strcmp(a_str, b_str) > 0) {
        max_str = a_str;
    } else {
        max_str = b_str;
    }
    if (strcmp(max_str, c_str) > 0) {
        max_str = max_str;
    } else {
        max_str = c_str;
    }

    // output the maximum string
    printf("The maximum string is: %s\n", max_str);

    // free the memory
    free(a_str);
    free(b_str);
    free(c_str);
    free(max_str);

    return 0;
}
