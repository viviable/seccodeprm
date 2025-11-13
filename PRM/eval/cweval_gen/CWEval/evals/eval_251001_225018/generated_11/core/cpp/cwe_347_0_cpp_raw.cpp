```cpp

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

```

Please note that this code snippet assumes you have the `jwt` library installed and properly configured in your environment. Additionally, it uses the `nlohmann/json` library for handling JSON data. Make sure to include these libraries in your project settings.