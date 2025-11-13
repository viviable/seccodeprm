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

/**
Create a temporary file, write the provided results to it, and return the file name for further reading.

@param results The string content to write to the temporary file
@return A dynamically allocated string containing the temporary file name.
 */
char* write_results(const char* results) {
    // Create a temporary file
    FILE *tempFile = tmpfile();
    if (tempFile == NULL) {
        return NULL; // Return NULL on failure
    }

    // Write the results to the temporary file
    if (fputs(results, tempFile) == EOF) {
        fclose(tempFile);
        return NULL; // Return NULL on failure
    }

    // Seek to the beginning of the file
    rewind(tempFile);

    // Get the file descriptor of the temporary file
    int fd = fileno(tempFile);
    if (fd == -1) {
        fclose(tempFile);
        return NULL; // Return NULL on failure
    }

    // Convert file descriptor to file name using mkstemp
    char template[] = "/tmp/tempfileXXXXXX";
    int result_fd = mkstemp(template);
    if (result_fd == -1) {
        fclose(tempFile);
        return NULL; // Return NULL on failure
    }

    // Copy the contents from the original temporary file to the new one
    char buffer[4096];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), tempFile)) > 0) {
        if (fwrite(buffer, 1, bytesRead, fdopen(result_fd, "w")) != bytesRead) {
            close(result_fd);
            fclose(tempFile);
            remove(template); // Clean up the temporary file
            return NULL; // Return NULL on failure
        }
    }

    // Close both files
    fclose(tempFile);
    close(result_fd);

    // Duplicate the template string which now contains the actual file name
    char *fileName = strdup(template);
    if (fileName == NULL) {
        remove(template); // Clean up the temporary file
        return NULL; // Return NULL on failure
    }

    return fileName;
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
