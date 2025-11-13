```c
#include <stdio.h>
#include <stdlib.h>
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

    // Append the prefix "File content: " to the buffer
    strcpy(buf, "File content: ");

    // Read the content of the file into the buffer starting from the end of the prefix
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
    out[BUF_SIZE - 1] = '\0';

    // Close the file descriptor
    close(fd);
}
```