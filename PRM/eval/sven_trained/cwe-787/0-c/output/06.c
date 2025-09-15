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
    char aStr[20];
    sprintf(aStr, "%.15f", a);
    char bStr[20];
    sprintf(bStr, "%.15f", b);
    char cStr[20];
    sprintf(cStr, "%.15f", c);

    // format the floats to string using the printf() function
    char dStr[20];
    printf("d = %20.15f\n", a);
    sscanf(dStr, "%f", &a);
    char eStr[20];
    printf("e = %20.15f\n", b);
    sscanf(eStr, "%f", &b);
    char fStr[20];
    printf("f = %20.15f\n", c);
    sscanf(fStr, "%f", &c);

    // use the string representations of the floats to compute the minimum
    // value of the three floats
    char *min = aStr;
    if (strcmp(bStr, min) < 0) {
        min = bStr;
    }
    if (strcmp(cStr, min) < 0) {
        min = cStr;
    }

    // print the minimum value
    printf("minimum = %s\n", min);
}
