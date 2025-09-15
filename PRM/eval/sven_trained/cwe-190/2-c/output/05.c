#include <stdio.h>
#include <stdlib.h>

extern int getMonthlySales(int month);

int main(int argc, char *argv[]) {
    // sum the number of sales for the first quarter
    int sales1 = getMonthlySales(1);
    int sales2 = getMonthlySales(2);
    int sales3 = getMonthlySales(3);

    printf("The sum of sales for the first quarter is: %d\n", sales1 + sales2 + sales3);

    // sum the number of sales for the second quarter
    int sales4 = getMonthlySales(4);
    int sales5 = getMonthlySales(5);
    int sales6 = getMonthlySales(6);

    printf("The sum of sales for the second quarter is: %d\n", sales4 + sales5 + sales6);

    return 0;
}
