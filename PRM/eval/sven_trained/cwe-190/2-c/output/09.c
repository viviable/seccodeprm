#include <stdio.h>
#include <stdlib.h>

extern int getMonthlySales(int month);

int main(int argc, char *argv[]) {
    // sum the number of sales for the first quarter of the year
    int sales = 0;
    for (int i = 1; i <= 3; i++) {
        sales += getMonthlySales(i);
    }
    printf("First quarter sales: %d\n", sales);

    // sum the number of sales for the second quarter of the year
    sales = 0;
    for (int i = 4; i <= 6; i++) {
        sales += getMonthlySales(i);
    }
    printf("Second quarter sales: %d\n", sales);

    return 0;
}
