```c
#include <string.h>

void set_values_in_arr(long long* values, int begin_index, int count) {
    if (values == NULL || begin_index < 0 || count <= 0) {
        return; // Handle invalid input gracefully
    }
    // Calculate the size of the memory block to zero out
    size_t bytes_to_zero = count * sizeof(*values);
    // Use memset to set the specified range of values to 0
    memset(values + begin_index, 0, bytes_to_zero);
}
```