```c
#include <string.h>

void set_values_in_arr(long long* values, int begin_index, int count) {
    if (values == NULL || begin_index < 0 || count <= 0) {
        return;
    }
    // Calculate the total size to be zeroed out
    size_t total_size = count * sizeof(long long);
    // Use memset to set the specified range of memory to zero
    memset(values + begin_index, 0, total_size);
}
```