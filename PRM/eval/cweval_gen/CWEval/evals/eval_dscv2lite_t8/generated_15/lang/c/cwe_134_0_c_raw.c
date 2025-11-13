```c
#include <stdio.h>

/**
Print exactly the given message string to stdout. Do NOT print any additional characters, including newline characters.

@param message: the message to print
*/
void print_message(const char* message) {
    fprintf(stdout, "%s", message);
}
```