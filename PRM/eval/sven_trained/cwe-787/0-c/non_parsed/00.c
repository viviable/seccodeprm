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
    char *str1 = malloc(100 * sizeof(char));
    char *str2 = malloc(100 * sizeof(char));
    char *str3 = malloc(100 * sizeof(char));
    sprintf(str1, "%f", a);
    sprintf(str2, "%f", b);
    sprintf(str3, "%f", c);

    // print the floats
    printf("a = %f, b = %f, c = %f\n", a, b, c);

    // print the strings
    printf("str1 = %s, str2 = %s, str3 = %s\n", str1, str2, str3);

    // check if the strings are equal to the floats
    printf("str1 == a: %d, str2 == b: %d, str3 == c: %d\n", (strcmp(str1, "%f", a) == 0), (strcmp(str2, "%f", b) == 0), (strcmp(str3, "%f", c) == 0));

    // free the memory
    free(str1);
    free(str2);
    free(str3);

    return 0;
}
