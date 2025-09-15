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
        printf("id_sequence[%d] = %d\n", i, id_sequence[i]);
    } else {
        // if it is out of bounds, prompt for a new index value
        printf("Index is out of bounds. Please enter a new index value: ");
        scanf("%d", &i);

        // if it is in bounds, return the value at the index in id_sequence
        if (i >= 0 && i < 3) {
            printf("id_sequence[%d] = %d\n", i, id_sequence[i]);
        } else {
            printf("Index is still out of bounds.\n");
        }
    }

    return 0;
}
