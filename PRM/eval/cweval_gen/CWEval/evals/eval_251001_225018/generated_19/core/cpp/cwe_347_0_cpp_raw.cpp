```cpp
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::string decode_json_web_token(const std::string& token, const std::string& key) {
    try {
        // Decode the JWT token using the HS256 algorithm
        auto decoded = jwt::decode(token, jwt::algorithm::hs256(key));

        // Extract the claims from the decoded token
        json claims = decoded.get_claims();

        // Convert the claims to a JSON string
        return claims.dump();
    } catch (...) {
        // Return an empty JSON object string if decoding fails
        return "{}";
    }
}
```