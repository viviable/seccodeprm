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
    char *fmt = "%e\n";
    char aStr[32];
    char bStr[32];
    char cStr[32];
    snprintf(aStr, sizeof(aStr), fmt, a);
    snprintf(bStr, sizeof(bStr), fmt, b);
    snprintf(cStr, sizeof(cStr), fmt, c);

    // print the formatted strings to standard output
    printf("a = %s\n", aStr);
    printf("b = %s\n", bStr);
    printf("c = %s\n", cStr);

    // parse the formatted strings back to floats using standard C library functions
    float a2 = (float)strtof(aStr, NULL);
    float b2 = (float)strtof(bStr, NULL);
    float c2 = (float)strtof(cStr, NULL);

    // print the parsed floats to standard output
    printf("a2 = %f\n", a2);
    printf("b2 = %f\n", b2);
    printf("c2 = %f\n", c2);

    // compare the original and parsed floats
    if (a != a2) {
        printf("Error: a != a2\n");
        return 1;
    }
    if (b != b2) {
        printf("Error: b != b2\n");
        return 1;
    }
    if (c != c2) {
        printf("Error: c != c2\n");
        return 1;
    }

    // print success message
    printf("Success: original and parsed floats are the same\n");
    return 0;
}
