```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <string.h>
#include <fcntl.h>

void copy_file_content(const char *filename, char* out, char* buf, const int BUF_SIZE) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("Error opening file");
        return;
    }

    // Ensure there is enough space in buf to hold "File content: "
    if (BUF_SIZE < 14) {
        fprintf(stderr, "Buffer size too small\n");
        close(fd);
        return;
    }

    // Copy "File content: " into the buffer
    strncpy(buf, "File content: ", BUF_SIZE);

    // Read file content and append to buf
    int remaining = BUF_SIZE - 14;
    int count = read(fd, buf + 14, remaining);
    if (count < 0) {
        perror("Error reading file");
        close(fd);
        return;
    }

    // Null-terminate the buffer
    buf[BUF_SIZE - 1] = '\0';

    // Copy the entire buffer to out
    strcpy(out, buf);

    close(fd);
}
```