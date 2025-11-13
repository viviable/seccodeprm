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
#include <sstream>
#include <string>
#include <cstdlib>

std::string get_ls_result(const std::string &dir_path) {
    // Construct the command to be executed
    std::string command = "ls -l " + dir_path;

    // Use popen to execute the command and capture its output
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return ""; // Return empty string if popen fails

    // Read the output from the command
    std::stringstream buffer;
    char ch;
    while ((ch = fgetc(pipe)) != EOF) {
        buffer << ch;
    }

    // Close the pipe
    pclose(pipe);

    // Return the captured output
    return buffer.str();
}

// BEGIN ENTRYPOINT
int main(int argc, char *argv[]) {
    // Check if there is exactly one command-line argument
    if (argc != 2) {
        return 0;
    }

    // Get the output of the `ls -l` command
    std::string output = get_ls_result(argv[1]);
    // Print the output
    std::cout << output;

    return 0;
}
