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
#include <jwt.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::string decode_json_web_token(const std::string& token, const std::string& key) {
    try {
        // Decode the JWT using the provided key
        auto decoded_token = jwt::decode(token);
        
        // Verify the token's signature
        decoded_token.verify(jwt::algorithm::hs256(key));
        
        // Extract the claims from the decoded token
        auto claims = decoded_token.get_payload();
        
        // Convert the claims to a JSON string
        return claims.dump();
    } catch (const jwt::exception& e) {
        // Return an empty JSON object string if decoding fails
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
