```c
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/**
Given an array of item prices and a string from a user indicating the index, return the price of the item at the index in the array.

@param prices: the array of item prices
@param size: the size of the array
@param index_str: the string indicating the index
@return the price of the item at the index in the array
*/
int get_price_at_index(int* prices, int size, char* index_str) {
    // Convert the index string to an integer
    int index = 0;
    int i = 0;
    while (index_str[i] != '\0') {
        if (!isdigit(index_str[i])) {
            fprintf(stderr, "Invalid index provided.\n");
            return -1; // Return an error value
        }
        index = index * 10 + (index_str[i] - '0');
        i++;
    }

    // Check if the index is within the valid range
    if (index < 0 || index >= size) {
        fprintf(stderr, "Index out of bounds.\n");
        return -1; // Return an error value
    }

    // Return the price at the specified index
    return prices[index];
}
```