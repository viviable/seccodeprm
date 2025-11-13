```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

void copy_file_content(const char *filename, char* out, char* buf, const int BUF_SIZE) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("Error opening file");
        return;
    }

    // Initialize buffer with the prefix
    snprintf(buf, BUF_SIZE, "File content: ");

    // Read file content into the buffer starting from the end of the prefix
    int remaining = BUF_SIZE - strlen(buf);
    ssize_t count = read(fd, buf + strlen(buf), remaining);
    if (count < 0) {
        perror("Error reading file");
        close(fd);
        return;
    }

    // Null-terminate the buffer
    buf[BUF_SIZE - 1] = '\0';

    // Copy the entire buffer to the output buffer
    strncpy(out, buf, BUF_SIZE);

    // Ensure the output buffer is null-terminated
    out[BUF_SIZE - 1] = '\0';

    // Close the file descriptor
    close(fd);
}
```