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
    while (*index_str && isspace((unsigned char)*index_str)) {
        index_str++;
    }
    if (*index_str == '-') {
        index = -1; // Handle negative indices
        index_str++;
    }
    while (*index_str && isdigit((unsigned char)*index_str)) {
        index = index * 10 + (*index_str - '0');
        index_str++;
    }

    // Check if the index is out of bounds
    if (index < 0 || index >= size) {
        fprintf(stderr, "Index out of bounds\n");
        exit(EXIT_FAILURE);
    }

    return prices[index];
}
```