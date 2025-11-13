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
#include <string>
#include <cstdio>
#include <cstring>

std::string create_private_key() {
    const int key_length = 2048; // Default RSA key length
    std::array<char, 1024 * 16> buffer{}; // Buffer to store the generated key
    FILE* pipe = popen(("openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:" + std::to_string(key_length) + " 2>/dev/null").c_str(), "r");
    if (!pipe) {
        std::cerr << "Failed to execute openssl command." << std::endl;
        return nullptr;
    }
    size_t bytesRead = fread(buffer.data(), 1, sizeof(buffer), pipe);
    pclose(pipe);
    if (bytesRead == 0) {
        std::cerr << "No data received from openssl command." << std::endl;
        return nullptr;
    }
    return std::string(buffer.data(), bytesRead);
}

// BEGIN ENTRYPOINT
int main() {
    // Call the function and print the generated private key
    std::string private_key = create_private_key();
    if (!private_key.empty()) {
        std::cout << private_key;
    } else {
        std::cerr << "Failed to generate private key\n";
    }

    return 0;
}
