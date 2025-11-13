```c
#include <string.h>

void set_values_in_arr(long long* values, int begin_index, int count) {
    if (values == NULL || count <= 0) {
        return;
    }
    // Calculate the total size of the memory to be reset
    size_t size = count * sizeof(long long);
    // Reset the values using memset
    memset(values + begin_index, 0, size);
}
```