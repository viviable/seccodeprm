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
    char a_str[100];
    sprintf(a_str, "%f", a);
    char b_str[100];
    sprintf(b_str, "%f", b);
    char c_str[100];
    sprintf(c_str, "%f", c);

    // determine the min and max values
    char *min_str, *max_str;
    if (a < b) {
        if (a < c) {
            min_str = a_str;
            if (b < c) {
                max_str = c_str;
            } else {
                max_str = b_str;
            }
        } else {
            min_str = c_str;
            max_str = b_str;
        }
    } else {
        if (b < c) {
            min_str = b_str;
            if (a < c) {
                max_str = c_str;
            } else {
                max_str = a_str;
            }
        } else {
            min_str = c_str;
            max_str = a_str;
        }
    }

    // print the min and max values to the console
    printf("Min value: %s\n", min_str);
    printf("Max value: %s\n", max_str);

    // compare the min and max values using the standard C library functions
    if (strcmp(min_str, max_str) == 0) {
        printf("The min and max values are the same.\n");
    } else {
        printf("The min and max values are different.\n");
    }

    return 0;
}
