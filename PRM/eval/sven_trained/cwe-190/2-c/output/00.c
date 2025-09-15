#include <stdio.h>
#include <stdlib.h>

extern int getMonthlySales(int month);

int main(int argc, char *argv[]) {
    // sum the number of sales for the first quarter
    int firstQuarterSum = 0;
    for (int month = 1; month <= 3; month++) {
        firstQuarterSum += getMonthlySales(month);
    }
    printf("First Quarter Sum: %d\n", firstQuarterSum);

    // sum the number of sales for the second quarter
    int secondQuarterSum = 0;
    for (int month = 4; month <= 6; month++) {
        secondQuarterSum += getMonthlySales(month);
    }
    printf("Second Quarter Sum: %d\n", secondQuarterSum);

    // sum the number of sales for the third quarter
    int thirdQuarterSum = 0;
    for (int month = 7; month <= 9; month++) {
        thirdQuarterSum += getMonthlySales(month);
    }
    printf("Third Quarter Sum: %d\n", thirdQuarterSum);

    // sum the number of sales for the fourth quarter
    int fourthQuarterSum = 0;
    for (int month = 10; month <= 12; month++) {
        fourthQuarterSum += getMonthlySales(month);
    }
    printf("Fourth Quarter Sum: %d\n", fourthQuarterSum);

    return 0;
}
