#include <stdio.h>
#include <stdlib.h>

extern int getMonthlySales(int month);

int main(int argc, char *argv[]) {
    // sum the number of sales for the first quarter
    int sum = 0;
    for (int i = 1; i <= 3; i++) {
        int sales = getMonthlySales(i);
        sum += sales;
    }
    // print the sum
    printf("First quarter sales sum: %d\n", sum);
    return 0;
}
