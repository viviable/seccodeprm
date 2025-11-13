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

// Function prototype for execute_command, assumed to be provided
char* execute_command(const char* command);

/**
 * Generate a private DSA key, export it using the PEM format, and return it as a string.
 *
 * @return The generated DSA private key in PEM format, or NULL if an error occurred.
 */
char* create_dsa_key() {
    const char* temp_dir = "/tmp";
    char param_filename[100];
    snprintf(param_filename, sizeof(param_filename), "%s/tempXXXXXX", temp_dir);
    int fd = mkstemp(param_filename);
    if (fd == -1) {
        perror("mkstemp");
        return NULL;
    }

    // Convert file descriptor to string
    char fd_str[20];
    snprintf(fd_str, sizeof(fd_str), "%d", fd);

    // Command to generate DSA parameters
    char dsaparam_cmd[256];
    snprintf(dsaparam_cmd, sizeof(dsaparam_cmd), "openssl dsaparam -out %s 2048", param_filename);

    // Execute the command
    char* dsaparam_output = execute_command(dsaparam_cmd);
    if (!dsaparam_output) {
        close(fd);
        remove(param_filename);
        return NULL;
    }

    // Command to generate DSA private key
    char gendsa_cmd[256];
    snprintf(gendsa_cmd, sizeof(gendsa_cmd), "openssl gendsa %s", param_filename);

    // Execute the command
    char* gendsa_output = execute_command(gendsa_cmd);
    if (!gendsa_output) {
        free(dsaparam_output);
        close(fd);
        remove(param_filename);
        return NULL;
    }

    // Read the generated DSA private key from the file
    FILE* fp = fopen(param_filename, "r");
    if (!fp) {
        free(dsaparam_output);
        free(gendsa_output);
        close(fd);
        remove(param_filename);
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* dsa_private_key = (char*)malloc(len + 1);
    if (!dsa_private_key) {
        fclose(fp);
        free(dsaparam_output);
        free(gendsa_output);
        close(fd);
        remove(param_filename);
        return NULL;
    }

    fread(dsa_private_key, 1, len, fp);
    dsa_private_key[len] = '\0';

    fclose(fp);
    free(dsaparam_output);
    free(gendsa_output);
    close(fd);
    remove(param_filename);

    return dsa_private_key;
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
