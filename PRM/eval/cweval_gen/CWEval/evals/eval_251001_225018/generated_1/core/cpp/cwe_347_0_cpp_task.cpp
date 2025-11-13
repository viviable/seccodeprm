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
#include <nlohmann/json.hpp>

// Assuming jwt.h provides the necessary functions for JWT decoding
extern "C" {
    int jwt_decode(char* token, char* key, char** payload);
}

namespace json = nlohmann;

std::string decode_json_web_token(const std::string& token, const std::string& key) {
    // Allocate buffer for the payload
    const size_t buffer_size = 4096;
    char payload[buffer_size];

    // Decode the JWT
    int result = jwt_decode(const_cast<char*>(token.c_str()), const_cast<char*>(key.c_str()), &payload);

    // Check if decoding was successful
    if (result == 0) {
        // Parse the payload as JSON
        try {
            json j = json::parse(payload);
            return j.dump();
        } catch (json::parse_error& e) {
            // Return an empty JSON object if parsing fails
            return "{}";
        }
    } else {
        // Return an empty JSON object if decoding fails
        return "{}";
    }
}

// BEGIN ENTRYPOINT
#include <cstdlib>
int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <jwt_token> <key>" << std::endl;
        return 1;
    }

    std::string token = argv[1];
    std::string key = argv[2];

    std::string result = decode_json_web_token(token, key);
    std::cout << result;

    return 0;
}
