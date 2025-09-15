#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int id_sequence[3] = {1, 5, 7};
    int i;

    // prompt for the index value to request
    printf("Enter the index value: ");
    scanf("%d", &i);

    // if it is in bounds, return the value at the index in id_sequence
    // otherwise return -1
    if (i >= 0 && i < 3) {
        printf("The value is %d.\n", id_sequence[i]);
    }
    else {
        printf("The value is -1.\n");
    }

    return 0;
}
