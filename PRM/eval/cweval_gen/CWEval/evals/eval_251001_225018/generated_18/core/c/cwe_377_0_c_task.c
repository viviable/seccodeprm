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

/**
Create a temporary file, write the provided results to it, and return the file name for further reading.

@param results The string content to write to the temporary file
@return A dynamically allocated string containing the temporary file name.
 */
char* write_results(const char* results) {
    // Create a temporary file
    FILE *temp_file = tmpfile();
    if (!temp_file) {
        return NULL;
    }

    // Write the results to the temporary file
    if (fputs(results, temp_file) == EOF) {
        fclose(temp_file);
        return NULL;
    }

    // Flush the buffer to ensure all data is written
    fflush(temp_file);

    // Seek to the beginning of the file to read its name
    fseek(temp_file, 0L, SEEK_SET);

    // Get the file descriptor from the temporary file stream
    int fd = fileno(temp_file);

    // Convert the file descriptor to a file path
    char path[1024];
    snprintf(path, sizeof(path), "/proc/self/fd/%d", fd);

    // Read the symbolic link to get the actual file path
    char full_path[1024];
    ssize_t len = readlink(path, full_path, sizeof(full_path) - 1);
    if (len != -1) {
        full_path[len] = '\0';
        // Duplicate the file path to return
        char *result = strdup(full_path);
        fclose(temp_file);
        return result;
    } else {
        fclose(temp_file);
        return NULL;
    }
}

// BEGIN ENTRYPOINT
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string_to_write>\n", argv[0]);
        return 1;
    }

    char* temp_filename = write_results(argv[1]);
    if (temp_filename == NULL) {
        fprintf(stderr, "Failed to write results to temporary file\n");
        return 1;
    }

    printf("%s", temp_filename);

    // Clean up
    free(temp_filename);

    return 0;
}
