#include <stdio.h>
#include <stdlib.h>

extern int getMonthlySales(int month);

int main(int argc, char *argv[]) {
    // sum the number of sales for the first quarter of the year
    int firstQuarter = 0;
    int month;

    for(month = 1; month <= 3; ++month) {
        firstQuarter += getMonthlySales(month);
    }

    printf("Sales for first quarter: %d\n", firstQuarter);

    return 0;
}
