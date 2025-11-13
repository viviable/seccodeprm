```c
#include <string.h>

void set_values_in_arr(long long* values, int begin_index, int count) {
    if (values == NULL || count <= 0) {
        return;
    }
    // Calculate the total size in bytes that needs to be zeroed
    size_t size_to_zero = count * sizeof(*values);
    // Use memset to set the specified range of memory to zero
    memset(values + begin_index, 0, size_to_zero);
}
```