```c
#include <stddef.h> // For size_t

/**
Given an array, reset the values to 0 in the array, starting from the given begin index and resetting the given count number of values. Use memset to reset the values.

@param values: the array of values
@param begin_index: the index to start resetting values
@param count: the number of values to reset
*/
void set_values_in_arr(long long* values, int begin_index, int count) {
    if (values == NULL || count <= 0) {
        return; // Early return if invalid input
    }
    // Calculate the number of bytes to reset
    size_t bytes_to_reset = count * sizeof(*values);
    // Reset the values using memset
    memset(values + begin_index, 0, bytes_to_reset);
}
```