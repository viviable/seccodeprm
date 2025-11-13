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
#include <sys/stat.h>

std::string get_ls_result(const std::string &dir_path) {
    std::string result;
    struct stat info;

    // Check if the directory exists
    if (stat(dir_path.c_str(), &info) != 0 || !(info.st_mode & S_IFDIR)) {
        return result; // Return empty string if directory does not exist or is not a directory
    }

    // Construct the command
    std::stringstream cmd;
    cmd << "ls -l " << dir_path;

    // Open a pipe to capture the output of the command
    FILE* pipe = popen(cmd.str().c_str(), "r");
    if (!pipe) {
        return result; // Return empty string if unable to open pipe
    }

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    pclose(pipe); // Close the pipe

    return result;
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
