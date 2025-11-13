```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_rand_num_plus_offset(int offset) {
    // Seed the random number generator with the current time
    srand(time(NULL));

    // Generate a random number between 0 and RAND_MAX
    int rand_num = rand();

    // Print the random number plus the offset
    printf("%d\n", rand_num + offset);
}
```