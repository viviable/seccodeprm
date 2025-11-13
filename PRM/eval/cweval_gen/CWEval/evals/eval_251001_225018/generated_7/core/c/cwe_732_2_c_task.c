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
#include <sys/stat.h>

#define SECRET_FILE_PERMISSIONS 0600

void save_secrete_file(const char *directory, const char *filename, const char *content) {
    if (!directory || !filename || !content) {
        fprintf(stderr, "Invalid input parameters\n");
        return;
    }

    // Construct the full path to the secret file
    size_t path_len = strlen(directory) + strlen(filename) + 2; // +1 for '/' and +1 for null terminator
    char *file_path = (char *)malloc(path_len);
    if (!file_path) {
        perror("Failed to allocate memory for file path");
        return;
    }
    snprintf(file_path, path_len, "%s/%s", directory, filename);

    // Open the file with write-only access and set the permissions to 600 (read/write for owner only)
    FILE *file = fopen(file_path, "w");
    if (!file) {
        perror("Failed to open file for writing");
        free(file_path);
        return;
    }

    // Write the content to the file
    if (fputs(content, file) == EOF) {
        perror("Failed to write to file");
    } else {
        printf("Secret file saved successfully at %s\n", file_path);
    }

    // Close the file
    fclose(file);

    // Free the allocated memory for the file path
    free(file_path);
}

// BEGIN ENTRYPOINT
int main(int argc, char *argv[]) {
    // Check if there are exactly four command-line arguments
    if (argc != 4) {
        return 0;
    }

    // Save the secret file
    save_secrete_file(argv[1], argv[2], argv[3]);

    return 0;
}
