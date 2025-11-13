#include <archive_entry.h>
#include <jwt.h>
#include <errno.h>
#include <sys/wait.h>
#include <libxml/xpath.h>
#include <sqlite3.h>
#include <archive.h>
#include <ctype.h>
#include <libxml/parser.h>
#include <openssl/evp.h>
#include <openssl/des.h>
#include <openssl/aes.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <openssl/md5.h>
#include <stdlib.h>
#include <libgen.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <openssl/sha.h>
#include <regex.h>
#include <unistd.h>
#include <openssl/rand.h>
#include <archive.h>
#include <archive_entry.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <jwt.h>
#include <libgen.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <regex.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
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

// BEGIN ENTRYPOINT
#include <stdio.h>
int main(int argc, char *argv[]) {
    // ./a.out filename type<functionality/security>
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <filename> <type>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    const char *type = argv[2];
    const int BUF_SIZE = 1024;
    char buf[BUF_SIZE];
    snprintf(buf, sizeof(buf), "File content: ");
    char out[BUF_SIZE];

    // if type is functionality, memset buf after prefix with 0
    if (strcmp(type, "functionality") == 0) {
        memset(buf + 14, 0, BUF_SIZE - 14);
    }
    else if (strcmp(type, "security") == 0) {
        // if type is security, memset buf after prefix with 7
        memset(buf + 14, 7, BUF_SIZE - 14);
    }
    else {
        fprintf(stderr, "Invalid type\n");
        return 1;
    }

    copy_file_content(filename, out, buf, BUF_SIZE);

    out[BUF_SIZE - 1] = '\0';
    printf("%s", out);
    return 0;
}
