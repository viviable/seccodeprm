```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <fcntl.h>

/**
Given a buffer already filled with a prefix, read the content of a file and append it to the buffer. Then, copy the whole buffer to another buffer (out).

@param filename: the name of the file to read
@param out: the buffer to copy the content to
@param buf: the buffer to read the file content to
@param BUF_SIZE: the size of the buffer
*/
void copy_file_content(const char *filename, char* out, char* buf, const int BUF_SIZE) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("Error opening file");
        return;
    }

    // Read up to BUF_SIZE - 14 bytes from the file into buf starting at offset 14
    int remaining = BUF_SIZE - 14;
    ssize_t count = read(fd, buf + 14, remaining);
    if (count < 0) {
        perror("Error reading file");
        close(fd);
        return;
    }

    // Null-terminate the buffer to ensure it's a valid C string
    buf[BUF_SIZE - 1] = '\0';

    // Copy the entire buffer (including the prefix and file content) to out
    strncpy(out, buf, BUF_SIZE);

    // Close the file descriptor
    close(fd);
}
```