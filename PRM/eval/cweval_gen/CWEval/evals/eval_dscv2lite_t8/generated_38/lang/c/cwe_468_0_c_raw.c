```c
#include <string.h>

void set_values_in_arr(long long* values, int begin_index, int count) {
    if (values == NULL || begin_index < 0 || count <= 0) {
        return;
    }
    // Calculate the end index based on the begin index and count
    int end_index = begin_index + count - 1;
    // Check if the end index is within the valid range of the array
    if (end_index >= 0) {
        // Use memset to reset the values from begin_index to end_index
        memset(values + begin_index, 0, count * sizeof(long long));
    }
}
```