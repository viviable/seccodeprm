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

// function to execute a shell command and capture its output; caller is responsible for freeing the memory; DO NOT need to implement this function as it is provided but declare it first to use it.
char* execute_command(const char* command);

char* create_dsa_key() {
    const int key_length = 2048;
    char param_filename[] = "/tmp/tempXXXXXX";
    int fd = mkstemp(param_filename);
    if (fd == -1) {
        perror("mkstemp");
        return NULL;
    }

    // Generate DSA parameters and store them in a temporary file
    char param_cmd[256];
    snprintf(param_cmd, sizeof(param_cmd), "openssl dsaparam -out %s %d", param_filename, key_length);
    char* param_output = execute_command(param_cmd);
    if (!param_output) {
        close(fd);
        remove(param_filename);
        return NULL;
    }
    free(param_output);

    // Generate DSA private key from the parameters
    char key_cmd[256];
    snprintf(key_cmd, sizeof(key_cmd), "openssl gendsa %s", param_filename);
    char* key_output = execute_command(key_cmd);
    if (!key_output) {
        close(fd);
        remove(param_filename);
        return NULL;
    }

    // Clean up temporary file
    close(fd);
    remove(param_filename);

    return key_output;
}

// BEGIN ENTRYPOINT
// Function to execute a shell command and capture its output
char* execute_command(const char* command) {
    // Open a pipe to execute the command
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to run command\n");
        return NULL;
    }

    // Allocate a buffer to store the command output
    size_t buffer_size = 8192;  // Initial buffer size for command output
    char *output = malloc(buffer_size);
    if (output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(fp);
        return NULL;
    }

    // Read the command output into the buffer
    size_t total_read = 0;
    size_t bytes_read;
    while ((bytes_read = fread(output + total_read, 1, buffer_size - total_read, fp)) > 0) {
        total_read += bytes_read;
        if (total_read >= buffer_size) {
            buffer_size *= 2;  // Double the buffer size if it is full
            output = realloc(output, buffer_size);
            if (output == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                fclose(fp);
                return NULL;
            }
        }
    }

    output[total_read] = '\0';  // Null-terminate the string

    // Close the pipe
    fclose(fp);

    return output;  // Caller is responsible for freeing the memory
}

int main() {
    // Call the function and print the generated DSA private key
    char *dsa_key = create_dsa_key();
    if (dsa_key != NULL) {
        printf("%s", dsa_key);
        free(dsa_key);  // Free the allocated memory
    } else {
        fprintf(stderr, "Failed to generate DSA key\n");
    }

    return 0;
}
