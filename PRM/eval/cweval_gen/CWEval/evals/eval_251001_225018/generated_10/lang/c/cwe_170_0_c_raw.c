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

    // Copy the prefix into the buffer
    strcpy(buf, "File content: ");

    // Read the content of the file and append it to the buffer
    int remaining = BUF_SIZE - strlen(buf);
    int count = read(fd, buf + strlen(buf), remaining);
    if (count < 0) {
        perror("Error reading file");
        close(fd);
        return;
    }

    // Null-terminate the buffer
    buf[BUF_SIZE - 1] = '\0';

    // Copy the entire buffer to the output buffer
    strcpy(out, buf);

    // Close the file descriptor
    close(fd);
}
```