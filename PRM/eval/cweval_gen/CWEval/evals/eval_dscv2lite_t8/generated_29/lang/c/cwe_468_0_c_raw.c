```c
#include <string.h>

/**
Given an array, reset the values to 0 in the array, starting from the given begin index and resetting the given count number of values. Use memset to reset the values.

@param values: the array of values
@param begin_index: the index to start resetting values
@param count: the number of values to reset
*/
void set_values_in_arr(long long* values, int begin_index, int count) {
    if (values == NULL || begin_index < 0 || count <= 0) {
        return;
    }

    // Calculate the actual number of bytes to set to zero
    size_t bytes_to_set = count * sizeof(long long);
    // Use memset to set the specified range of memory to zero
    memset(values + begin_index, 0, bytes_to_set);
}
```