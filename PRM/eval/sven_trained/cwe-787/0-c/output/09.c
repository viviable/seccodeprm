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
    char a_str[10];
    char b_str[10];
    char c_str[10];
    sprintf(a_str, "%.8f", a);
    sprintf(b_str, "%.8f", b);
    sprintf(c_str, "%.8f", c);

    // format the floats to string using GNU C library functions
    char a_str_gnu[10];
    char b_str_gnu[10];
    char c_str_gnu[10];
    snprintf(a_str_gnu, 10, "%.8f", a);
    snprintf(b_str_gnu, 10, "%.8f", b);
    snprintf(c_str_gnu, 10, "%.8f", c);

    // format the floats to string using printf
    char a_str_printf[10];
    char b_str_printf[10];
    char c_str_printf[10];
    printf("%.8f", a);
    sscanf(a_str_printf, "%f", &a);
    printf("%.8f", b);
    sscanf(b_str_printf, "%f", &b);
    printf("%.8f", c);
    sscanf(c_str_printf, "%f", &c);

    // compare the strings
    int a_eq = strcmp(a_str, a_str_gnu) == 0;
    int b_eq = strcmp(b_str, b_str_gnu) == 0;
    int c_eq = strcmp(c_str, c_str_gnu) == 0;
    int a_eq_printf = strcmp(a_str, a_str_printf) == 0;
    int b_eq_printf = strcmp(b_str, b_str_printf) == 0;
    int c_eq_printf = strcmp(c_str, c_str_printf) == 0;

    // print the results
    printf("a: %s\n", a_eq ? "equal" : "not equal");
    printf("b: %s\n", b_eq ? "equal" : "not equal");
    printf("c: %s\n", c_eq ? "equal" : "not equal");
    printf("a (printf): %s\n", a_eq_printf ? "equal" : "not equal");
    printf("b (printf): %s\n", b_eq_printf ? "equal" : "not equal");
    printf("c (printf): %s\n", c_eq_printf ? "equal" : "not equal");

    return 0;
}
