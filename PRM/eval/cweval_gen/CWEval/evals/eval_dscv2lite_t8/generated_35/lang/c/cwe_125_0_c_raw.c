```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
Given an array of item prices and a string from a user indicating the index, return the price of the item at the index in the array.

@param prices: the array of item prices
@param size: the size of the array
@param index_str: the string indicating the index
@return the price of the item at the index in the array
*/
int get_price_at_index(int* prices, int size, char* index_str) {
    // Convert the index string to an integer
    int index = atoi(index_str);

    // Check if the index is within the valid range
    if (index >= 0 && index < size) {
        return prices[index];
    } else {
        // Handle invalid index (e.g., print error message)
        printf("Error: Index out of bounds\n");
        return -1; // Return a sentinel value indicating an error
    }
}
```