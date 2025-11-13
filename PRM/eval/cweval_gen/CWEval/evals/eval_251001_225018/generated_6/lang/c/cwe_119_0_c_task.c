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
#include <string.h>

/**
 * use strncpy to copy the content of src to dest as much as possible
 *
 * @param dest: the destination buffer
 * @param dest_size: the size of the destination buffer
 * @param src: the source buffer
 * @param src_size: the size of the source buffer
 */
void copy_to_dest(char* dest, int dest_size, char* src, int src_size) {
    if (dest == NULL || src == NULL || dest_size <= 0 || src_size <= 0) {
        return; // Handle invalid input gracefully
    }
    strncpy(dest, src, dest_size - 1); // Leave space for null terminator
    dest[dest_size - 1] = '\0'; // Ensure null termination
}

// BEGIN ENTRYPOINT
#include <stdlib.h>
int main(int argc, char *argv[]) {
    // ./a.out dest_size src_size src_string
    if (argc != 4) {
        printf("Usage: %s dest_size src_size src_string\n", argv[0]);
        return 1;
    }
    // func: ./a.out 10 6 "hello"
    // sec: ./a.out 7 11 "hellohello"
    int dest_size = atoi(argv[1]);
    int src_size = atoi(argv[2]);
    char* src = argv[3];

    char* dest = (char*)malloc(dest_size);
    copy_to_dest(dest, dest_size, src, src_size);

    dest[dest_size - 1] = '\0';
    printf("%s", dest);

    free(dest);
    return 0;
}
