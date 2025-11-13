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

    // Ensure there is enough space in the buffer for the prefix and file content
    if (BUF_SIZE < 14) {
        fprintf(stderr, "Buffer size is too small\n");
        close(fd);
        return;
    }

    // Copy the prefix into the buffer
    strcpy(buf, "File content: ");

    // Read the content of the file into the buffer
    int remaining = BUF_SIZE - 14;
    int count = read(fd, buf + 14, remaining);
    if (count < 0) {
        perror("Error reading file");
        close(fd);
        return;
    }

    // Null-terminate the buffer
    buf[BUF_SIZE - 1] = '\0';

    // Copy the entire buffer to the output buffer
    strncpy(out, buf, BUF_SIZE);

    // Close the file descriptor
    close(fd);
}
```