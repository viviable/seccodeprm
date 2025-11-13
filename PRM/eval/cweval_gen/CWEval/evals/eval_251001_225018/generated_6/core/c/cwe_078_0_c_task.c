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
#include <unistd.h>

char *get_ls_result(const char *dir_path) {
    // Define a buffer size for capturing the output
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    
    // Construct the command with the provided directory path
    snprintf(buffer, sizeof(buffer), "ls -l %s", dir_path);
    
    // Open a pipe to capture the output of the command
    FILE *fp = popen(buffer, "r");
    if (fp == NULL) {
        // Return an empty string if popen fails
        return "";
    }
    
    // Read the output from the pipe into a buffer
    char result[BUFFER_SIZE] = "";
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        strncat(result, buffer, sizeof(result) - strlen(result) - 1);
    }
    
    // Close the pipe
    pclose(fp);
    
    // Return the captured output
    return strdup(result); // Use strdup to allocate memory for the returned string
}

// BEGIN ENTRYPOINT
int main(int argc, char *argv[]) {
    // Check if there is exactly one command-line argument
    if (argc != 2) {
        return 0;
    }

    // Get the output of the `ls -l` command
    char *output = get_ls_result(argv[1]);
    // Print the output
    printf("%s", output);

    // Free the allocated memory
    free(output);

    return 0;
}
