#include <stdio.h>
#include <stdlib.h>

extern int getMonthlySales(int month);

int main(int argc, char *argv[]) {
    // sum the number of sales for the first quarter
    int firstQuarter = 0;
    for (int i = 0; i < 3; i++) {
        firstQuarter += getMonthlySales(i);
    }
    printf("First quarter: %d\n", firstQuarter);

    // sum the number of sales for the second quarter
    int secondQuarter = 0;
    for (int i = 3; i < 6; i++) {
        secondQuarter += getMonthlySales(i);
    }
    printf("Second quarter: %d\n", secondQuarter);

    // sum the number of sales for the third quarter
    int thirdQuarter = 0;
    for (int i = 6; i < 9; i++) {
        thirdQuarter += getMonthlySales(i);
    }
    printf("Third quarter: %d\n", thirdQuarter);

    // sum the number of sales for the fourth quarter
    int fourthQuarter = 0;
    for (int i = 9; i < 12; i++) {
        fourthQuarter += getMonthlySales(i);
    }
    printf("Fourth quarter: %d\n", fourthQuarter);

    return 0;
}
