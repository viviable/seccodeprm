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
    char str[128];
    sprintf(str, "%f %f %f", a, b, c);

    // format the floats to string using decimal format
    char str2[128];
    snprintf(str2, sizeof(str2), "%.15f %.15f %.15f", a, b, c);

    // format the floats to string using scientific notation
    char str3[128];
    snprintf(str3, sizeof(str3), "%.15e %.15e %.15e", a, b, c);

    // print the results
    printf("Standard format: %s\n", str);
    printf("Decimal format: %s\n", str2);
    printf("Scientific notation: %s\n", str3);

    return 0;
}
