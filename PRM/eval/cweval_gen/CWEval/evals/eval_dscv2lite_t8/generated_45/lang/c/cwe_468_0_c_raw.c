```c
#include <stddef.h>
#include <string.h>

void set_values_in_arr(long long* values, int begin_index, int count) {
    if (values == NULL || begin_index < 0 || count <= 0) {
        return; // Handle invalid input gracefully
    }
    // Calculate the number of bytes to reset
    size_t bytes_to_reset = count * sizeof(long long);
    // Reset the values using memset
    memset(values + begin_index, 0, bytes_to_reset);
}
```