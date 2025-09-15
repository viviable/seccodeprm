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
    char *format_a = malloc(50 * sizeof(char));
    sprintf(format_a, "%f", a);
    char *format_b = malloc(50 * sizeof(char));
    sprintf(format_b, "%f", b);
    char *format_c = malloc(50 * sizeof(char));
    sprintf(format_c, "%f", c);

    // print the result
    printf("%s\n", format_a);
    printf("%s\n", format_b);
    printf("%s\n", format_c);

    // print the result of using the -f option
    printf("Using -f option:\n");
    float d = a + b + c;
    printf("%f\n", d);

    // print the result of using the -m option
    printf("Using -m option:\n");
    float e = a * b * c;
    printf("%f\n", e);

    // free the allocated memory
    free(format_a);
    free(format_b);
    free(format_c);

    return 0;
}
