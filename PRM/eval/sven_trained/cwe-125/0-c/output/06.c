#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int id_sequence[3] = {1, 5, 7};
    int i;

    // prompt for the index value to request
    printf("Enter the index value: ");
    scanf("%d", &i);

    // if it is in bounds, return the value at the index in id_sequence
    // otherwise, return -1 to indicate an error
    printf("%d\n", (i >= 0 && i < 3) ? id_sequence[i] : -1);

    return 0;
}
