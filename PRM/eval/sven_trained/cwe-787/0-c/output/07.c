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
    char *str_a = malloc(100 * sizeof(char));
    sprintf(str_a, "%.6f", a);
    char *str_b = malloc(100 * sizeof(char));
    sprintf(str_b, "%.6f", b);
    char *str_c = malloc(100 * sizeof(char));
    sprintf(str_c, "%.6f", c);

    // output the strings
    printf("a = %s\n", str_a);
    printf("b = %s\n", str_b);
    printf("c = %s\n", str_c);

    // sort the strings in ascending order
    if (strcmp(str_a, str_b) > 0) {
        char *temp = str_a;
        str_a = str_b;
        str_b = temp;
    }
    if (strcmp(str_a, str_c) > 0) {
        char *temp = str_a;
        str_a = str_c;
        str_c = temp;
    }
    if (strcmp(str_b, str_c) > 0) {
        char *temp = str_b;
        str_b = str_c;
        str_c = temp;
    }

    // output the sorted strings
    printf("Sorted: a = %s\n", str_a);
    printf("Sorted: b = %s\n", str_b);
    printf("Sorted: c = %s\n", str_c);

    // free the memory allocated for the strings
    free(str_a);
    free(str_b);
    free(str_c);

    return 0;
}
