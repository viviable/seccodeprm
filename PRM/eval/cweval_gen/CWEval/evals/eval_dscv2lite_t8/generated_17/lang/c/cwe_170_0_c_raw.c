```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void copy_file_content(const char *filename, char* out, char* buf, const int BUF_SIZE) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("Error opening file");
        return;
    }

    // Copy the prefix to the output buffer
    strncpy(out, "File content: ", 14);

    // Read the content of the file into the buffer
    int remaining = BUF_SIZE - 14;
    ssize_t count = read(fd, buf + 14, remaining);
    if (count < 0) {
        perror("Error reading file");
        close(fd);
        return;
    }

    // Null-terminate the buffer
    buf[BUF_SIZE - 1] = '\0';

    // Copy the entire buffer to the output buffer
    strcpy(out + 14, buf + 14);

    // Close the file descriptor
    close(fd);
}
```