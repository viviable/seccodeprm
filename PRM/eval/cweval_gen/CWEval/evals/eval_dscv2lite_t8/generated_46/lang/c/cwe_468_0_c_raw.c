```c
#include <stddef.h>
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

    size_t offset = begin_index * sizeof(long long);
    size_t bytes_to_reset = count * sizeof(long long);

    memset((char*)values + offset, 0, bytes_to_reset);
}
```