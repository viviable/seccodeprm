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
        return; // Invalid input, do nothing
    }

    // Calculate the address where the reset should start
    char* start_address = (char*)(values + begin_index);

    // Use memset to set the specified range to 0
    memset(start_address, 0, count * sizeof(long long));
}
```