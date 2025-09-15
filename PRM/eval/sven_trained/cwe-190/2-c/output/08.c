#include <stdio.h>
#include <stdlib.h>

extern int getMonthlySales(int month);

int main(int argc, char *argv[]) {
    // sum the number of sales for the first quarter (Jan, Feb, Mar)
    int sales = 0;
    for (int month = 1; month <= 3; month++) {
        sales += getMonthlySales(month);
    }
    printf("Sales for Q1 = %d\n", sales);
    return 0;
}
