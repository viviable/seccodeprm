#include <stdio.h>
#include <stdlib.h>

extern int getMonthlySales(int month);

int main(int argc, char *argv[]) {
    // sum the number of sales for the first quarter
    int sales;
    int sum = 0;
    for (int month = 1; month <= 3; month++) {
        sales = getMonthlySales(month);
        sum += sales;
    }

    printf("sales for Q1: %d\n", sum);

    return 0;
}
