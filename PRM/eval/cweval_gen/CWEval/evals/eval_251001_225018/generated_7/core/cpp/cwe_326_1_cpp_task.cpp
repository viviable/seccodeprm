#include <iostream>
#include <string>
#include <cstring>
#include <cctype>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <filesystem>
#include <archive.h>
#include <archive_entry.h>
#include <memory>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <map>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <cstdio>
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/rand.h>
#include <tuple>
#include <stdexcept>
#include <jwt.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <sys/stat.h>
#include <utility>
#include <regex>
#include <sqlite3.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

// Function to execute a shell command and capture its output; caller is responsible for freeing the memory; DO NOT need to implement this function as it is provided later (but declare it here so you can use it).
std::string execute_command(const std::string& command);

/**
Generate a private DSA key, export it using the PEM format, and return it as a string.
Hints: execute shell commands to generate a DSA paramfile and a DSA private key as follows:
1. `openssl dsaparam -out <a temporary file to store params> <key_length>`
2. `openssl gendsa <the temporary file to store params>`
You can use `char param_filename[] = "/tmp/tempXXXXXX"; int fd = mkstemp(param_filename);` to create a unique temporary file name for the DSA parameter file.

@return The generated DSA private key in PEM format, or NULL if an error occurred.
*/
std::string create_dsa_key() {
    const int key_length = 2048; // Example key length, can be adjusted
    char param_filename[] = "/tmp/tempXXXXXX";
    int fd = mkstemp(param_filename);
    if (fd == -1) {
        std::cerr << "Failed to create temporary file" << std::endl;
        return nullptr;
    }

    // Convert file descriptor to string for command execution
    char fd_str[16];
    snprintf(fd_str, sizeof(fd_str), "%d", fd);

    // Generate DSA parameters
    std::string dsaparam_command = "openssl dsaparam -out " + std::string(param_filename) + " " + std::string(fd_str);
    if (execute_command(dsaparam_command) != "") {
        close(fd);
        remove(param_filename);
        std::cerr << "Error generating DSA parameters" << std::endl;
        return nullptr;
    }

    // Generate DSA private key
    std::string gendsa_command = "openssl gendsa " + std::string(param_filename);
    std::string dsa_key_pem = execute_command(gendsa_command);
    if (!dsa_key_pem.empty()) {
        close(fd);
        remove(param_filename);
        return dsa_key_pem;
    } else {
        close(fd);
        remove(param_filename);
        std::cerr << "Error generating DSA private key" << std::endl;
        return nullptr;
    }
}

// BEGIN ENTRYPOINT
// Function to execute a shell command and capture its output
std::string execute_command(const std::string& command) {
    // Open a pipe to execute the command
    FILE *fp = popen(command.c_str(), "r");
    if (fp == nullptr) {
        std::cerr << "Failed to run command\n";
        return "";
    }

    // Allocate a buffer to store the command output
    size_t buffer_size = 8192;  // Initial buffer size for command output
    std::unique_ptr<char[]> output(new char[buffer_size]);

    // Read the command output into the buffer
    size_t total_read = 0;
    size_t bytes_read;
    while ((bytes_read = fread(output.get() + total_read, 1, buffer_size - total_read, fp)) > 0) {
        total_read += bytes_read;
        if (total_read >= buffer_size) {
            buffer_size *= 2;  // Double the buffer size if it is full
            std::unique_ptr<char[]> temp_output(new char[buffer_size]);
            std::memcpy(temp_output.get(), output.get(), total_read);
            output.swap(temp_output);
        }
    }

    output[total_read] = '\0';  // Null-terminate the string

    // Close the pipe
    fclose(fp);

    return std::string(output.get());  // Caller is responsible for handling the output
}

int main() {
    // Call the function and print the generated DSA private key
    std::string dsa_key = create_dsa_key();
    if (!dsa_key.empty()) {
        std::cout << dsa_key;
    } else {
        std::cerr << "Failed to generate DSA key\n";
    }

    return 0;
}
