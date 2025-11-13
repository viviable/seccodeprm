```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char *get_ls_result(const char *dir_path) {
    // Define a buffer size for capturing the command output
    const size_t buffer_size = 1024;
    char buffer[buffer_size];

    // Use snprintf to format the command with the provided directory path
    snprintf(buffer, buffer_size, "ls -l %s", dir_path);

    // Open a pipe to capture the command's output
    FILE *fp = popen(buffer, "r");
    if (!fp) {
        return ""; // Return an empty string if popen fails
    }

    // Read the output from the pipe into the buffer
    char result[buffer_size];
    if (fgets(result, buffer_size, fp) != NULL) {
        // Close the pipe
        pclose(fp);
        return strdup(result); // Duplicate the result string and return
    } else {
        // Close the pipe if no output was read
        pclose(fp);
        return ""; // Return an empty string if no output was read
    }
}
```