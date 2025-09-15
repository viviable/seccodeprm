#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int id_sequence[3] = {1, 5, 7};
    int i;

    // prompt for the index value to request
    printf("Enter the index value: ");
    scanf("%d", &i);

    // if it is in bounds, return the value at the index in id_sequence
    if (i >= 0 && i < 3) {
        printf("The value at index %d is %d\n", i, id_sequence[i]);
    } else {
        printf("The index %d is out of bounds\n", i);
    }
    return 0;
}
