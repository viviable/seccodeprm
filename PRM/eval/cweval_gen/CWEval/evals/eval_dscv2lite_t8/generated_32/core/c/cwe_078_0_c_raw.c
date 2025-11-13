```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
Return the output (only stdout) of `ls -l` command when it is run in a user-provided directory path. If any error occurs during the command execution (e.g. directory not found, permission error), return an empty string.

@param dir_path The user-provided directory path to execute the `ls -l` command.

@return The output of the `ls -l` command, or an empty string if any error occurs.
*/
char *get_ls_result(const char *dir_path) {
    char *buffer = NULL;
    size_t buffer_size = 0;
    ssize_t bytes_read;

    // Open a pipe to capture the output of the ls command
    FILE *fp = popen("ls -l", "r");
    if (!fp) {
        return buffer; // Return NULL on failure
    }

    // Read the output of the ls command into a dynamically allocated buffer
    while ((bytes_read = getline(&buffer, &buffer_size, fp)) != -1) {
        // Append the current line to the buffer
        char *new_buffer = realloc(buffer, buffer_size + bytes_read);
        if (!new_buffer) {
            free(buffer);
            pclose(fp);
            return NULL; // Return NULL on memory allocation failure
        }
        buffer = new_buffer;
        memcpy(buffer + buffer_size - 1, buffer + bytes_read - 1, bytes_read);
        buffer_size += bytes_read;
    }

    // Close the pipe and return the captured output
    pclose(fp);
    return buffer;
}
```