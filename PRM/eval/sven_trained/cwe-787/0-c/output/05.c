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
    char *aStr = malloc(20 * sizeof(char));
    sprintf(aStr, "%.1f", a);
    char *bStr = malloc(20 * sizeof(char));
    sprintf(bStr, "%.1f", b);
    char *cStr = malloc(20 * sizeof(char));
    sprintf(cStr, "%.1f", c);

    // format the floats to string using custom functions
    char *aStrCustom = malloc(20 * sizeof(char));
    aStrCustom = floatToHex(a, aStrCustom);
    char *bStrCustom = malloc(20 * sizeof(char));
    bStrCustom = floatToHex(b, bStrCustom);
    char *cStrCustom = malloc(20 * sizeof(char));
    cStrCustom = floatToHex(c, cStrCustom);

    // print the formatted strings
    printf("a = %s\n", aStr);
    printf("b = %s\n", bStr);
    printf("c = %s\n", cStr);
    printf("a (custom) = %s\n", aStrCustom);
    printf("b (custom) = %s\n", bStrCustom);
    printf("c (custom) = %s\n", cStrCustom);

    // free the memory
    free(aStr);
    free(bStr);
    free(cStr);
    free(aStrCustom);
    free(bStrCustom);
    free(cStrCustom);

    return 0;
}
