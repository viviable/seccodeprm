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
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    // Create the full path for the secret file
    fs::path fullPath = directory / filename;

    // Ensure the directory exists, create if not
    if (!fs::exists(directory)) {
        fs::create_directories(directory);
    }

    // Open the file with write-only access, fail if it already exists
    std::ofstream outFile(fullPath, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!outFile.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + fullPath.string());
    }

    // Write the content to the file
    outFile.write(content.c_str(), content.size());

    // Close the file
    outFile.close();

    // Set file permissions to read/write only for the owner
    if (!fs::permissions(fullPath, fs::perms::owner_read | fs::perms::owner_write)) {
        throw std::runtime_error("Failed to set file permissions");
    }
}

// BEGIN ENTRYPOINT
int main(int argc, char* argv[]) {
    // Check if there are exactly four command-line arguments
    if (argc != 4) {
        return 1;
    }

    // Save the secret file
    saveSecreteFile(argv[1], argv[2], argv[3]);

    return 0;
}
