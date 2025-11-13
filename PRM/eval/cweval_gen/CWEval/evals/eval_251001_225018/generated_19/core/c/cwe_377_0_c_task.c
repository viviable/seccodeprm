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
    FILE *temp_file = tmpfile();
    if (temp_file == NULL) {
        perror("Failed to create temporary file");
        return NULL;
    }

    // Write the results to the temporary file
    if (fputs(results, temp_file) == EOF) {
        perror("Failed to write to temporary file");
        fclose(temp_file);
        return NULL;
    }

    // Flush the buffer to ensure data is written
    fflush(temp_file);

    // Get the file descriptor of the temporary file
    int fd = fileno(temp_file);

    // Allocate memory for the file name
    char *file_name = malloc(1024); // Assuming the file name will not exceed 1023 characters
    if (file_name == NULL) {
        perror("Failed to allocate memory for file name");
        fclose(temp_file);
        return NULL;
    }

    // Get the absolute path of the temporary file
    if (fgets(file_name, 1024, temp_file) != NULL) {
        // Remove the newline character if present
        size_t len = strlen(file_name);
        if (len > 0 && file_name[len - 1] == '\n') {
            file_name[len - 1] = '\0';
        }
    } else {
        perror("Failed to read file name from temporary file");
        free(file_name);
        fclose(temp_file);
        return NULL;
    }

    // Close the temporary file
    fclose(temp_file);

    return file_name;
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
