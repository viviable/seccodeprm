```c
#include <string.h>
#include <fcntl.h>

void copy_file_content(const char *filename, char* out, char* buf, const int BUF_SIZE) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("Error opening file");
        return;
    }

    // strlen("File content: ") == 14
    // buf = "File content: <...unused space...>"
    // read file content to buf at offset 14
    int remaining = BUF_SIZE - 14;
    int count = read(fd, buf + 14, remaining);
    if (count < 0) {
        perror("Error reading file");
        close(fd);
        return;
    }

    // Copy the prefix "File content: " into buf
    strncpy(buf, "File content: ", 14);

    // Ensure null-termination of buf
    buf[BUF_SIZE - 1] = '\0';

    // Calculate the total length of the content including the prefix
    int total_length = 14 + count;

    // Copy the entire buffer content to out
    memcpy(out, buf, total_length);

    // Null-terminate the output buffer
    out[total_length] = '\0';

    // Close the file descriptor
    close(fd);
}
```