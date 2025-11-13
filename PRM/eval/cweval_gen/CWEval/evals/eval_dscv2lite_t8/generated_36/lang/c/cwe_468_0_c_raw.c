```c
#include <string.h>

void set_values_in_arr(long long* values, int begin_index, int count) {
    if (values == NULL || begin_index < 0 || count <= 0) {
        return;
    }
    // Calculate the end index based on the begin_index and count
    int end_index = begin_index + count - 1;
    // Ensure the end_index is within the bounds of the array
    if (end_index >= INT_MAX / sizeof(long long)) {
        return;
    }
    // Calculate the total size in bytes to be reset
    size_t size_to_reset = (size_t)(end_index - begin_index + 1) * sizeof(long long);
    // Reset the values using memset
    memset(&values[begin_index], 0, size_to_reset);
}
```