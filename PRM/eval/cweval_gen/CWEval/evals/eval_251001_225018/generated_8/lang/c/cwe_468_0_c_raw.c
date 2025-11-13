```c
#include <string.h>

void set_values_in_arr(long long* values, int begin_index, int count) {
    if (values == NULL || begin_index < 0 || count <= 0) {
        return;
    }
    // Calculate the end index for the range to be reset
    int end_index = begin_index + count - 1;
    // Check if the end index is within the bounds of the array
    if (end_index >= INT_MAX / sizeof(long long)) {
        return;
    }
    // Reset the values using memset
    memset(values + begin_index, 0, count * sizeof(long long));
}
```