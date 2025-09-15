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
    char a_str[50];
    char b_str[50];
    char c_str[50];
    snprintf(a_str, sizeof(a_str), "%f", a);
    snprintf(b_str, sizeof(b_str), "%f", b);
    snprintf(c_str, sizeof(c_str), "%f", c);

    // get the maximum and minimum values from the 3 floats
    float max = a;
    float min = a;
    if (b > max) {
        max = b;
    }
    if (b < min) {
        min = b;
    }
    if (c > max) {
        max = c;
    }
    if (c < min) {
        min = c;
    }

    // print the results
    printf("a = %f\n", a);
    printf("b = %f\n", b);
    printf("c = %f\n", c);
    printf("max = %f\n", max);
    printf("min = %f\n", min);
    printf("a_str = %s\n", a_str);
    printf("b_str = %s\n", b_str);
    printf("c_str = %s\n", c_str);

    return 0;
}
