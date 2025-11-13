```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// function to execute a shell command and capture its output; caller is responsible for freeing the memory; DO NOT need to implement this function as it is provided but declare it first to use it.
char* execute_command(const char* command);

/**
Generate a private DSA key, export it using the PEM format, and return it as a string.
Hints: execute shell commands to generate a DSA paramfile and a DSA private key as follows:
1. `openssl dsaparam -out <a temporary file to store params> <key_length>`
2. `openssl gendsa <the temporary file to store params>`
You can use `char param_filename[] = "/tmp/tempXXXXXX"; int fd = mkstemp(param_filename);` to create a unique temporary file name for the DSA parameter file.

@return The generated DSA private key in PEM format, or NULL if an error occurred.
*/
char* create_dsa_key() {
    const char* temp_param_file = "/tmp/tempXXXXXX";
    int fd = mkstemp((char*)temp_param_file);
    if (fd == -1) {
        perror("mkstemp");
        return NULL;
    }

    // Convert file descriptor to string for command execution
    char fd_str[30];
    snprintf(fd_str, sizeof(fd_str), "%d", fd);

    // Generate DSA parameters
    char gen_params_cmd[1024];
    snprintf(gen_params_cmd, sizeof(gen_params_cmd), "openssl dsaparam -out %s 2048 > /dev/null 2>&1", temp_param_file);
    if (execute_command(gen_params_cmd) != NULL) {
        close(fd);
        remove(temp_param_file);
        return NULL;
    }

    // Generate DSA private key
    char gen_key_cmd[1024];
    snprintf(gen_key_cmd, sizeof(gen_key_cmd), "openssl gendsa %s > /dev/null 2>&1", temp_param_file);
    if (execute_command(gen_key_cmd) != NULL) {
        close(fd);
        remove(temp_param_file);
        return NULL;
    }

    // Read the generated DSA private key from the file
    char* dsa_key = execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----END DSA PRIVATE KEY-----\n", "", 'g', execute_command(gensub("^.*-----BEGIN DSA