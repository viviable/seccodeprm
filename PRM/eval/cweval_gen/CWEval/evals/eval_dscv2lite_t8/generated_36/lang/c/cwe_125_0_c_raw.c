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
    char* end;
    long index = strtol(index_str, &end, 10);
    
    // Check if the conversion was successful and the index is within bounds
    if (*end != '\0' || index < 0 || index >= size) {
        fprintf(stderr, "Invalid index provided.\n");
        exit(EXIT_FAILURE);
    }
    
    // Return the price at the specified index
    return prices[index];
}
```