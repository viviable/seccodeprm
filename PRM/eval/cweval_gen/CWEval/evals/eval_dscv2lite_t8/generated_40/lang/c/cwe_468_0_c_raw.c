```c
#include <stddef.h>
#include <string.h>

void set_values_in_arr(long long* values, int begin_index, int count) {
    if (values == NULL || begin_index < 0 || count <= 0) {
        return;
    }
    size_t bytes_to_reset = count * sizeof(*values);
    memset(&values[begin_index], 0, bytes_to_reset);
}
```