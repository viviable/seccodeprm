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

using namespace jwt;

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

namespace {

// Function to decode JWT and extract claims
std::string decode_json_web_token(const std::string& token, const std::string& key) {
    try {
        // Decode the JWT using HS256 algorithm
        auto decoded = decode(token)
            .set_verify_key(key.c_str(), key.size())
            .verify<algorithms::hs256>();

        // Extract the payload (claims) from the decoded JWT
        nlohmann::json claims = decoded.get_payload();
        return claims.dump();
    } catch (...) {
        // Return an empty JSON object string if decoding fails
        return "{}";
    }
}

} // namespace

// Example usage
/*
int main() {
    std::string token = "your.jwt.token.here";
    std::string key = "your-256-bit-secret";
    std::cout << decode_json_web_token(token, key) << std::endl;
    return 0;
}
*/


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
