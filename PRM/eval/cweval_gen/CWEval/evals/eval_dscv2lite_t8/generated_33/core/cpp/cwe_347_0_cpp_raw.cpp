```cpp

#include <jwt.h>
#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::string decode_json_web_token(const std::string& token, const std::string& key) {
    try {
        // Decode the JWT using HS256 algorithm
        auto decoded_token = jwt::decode(token, jwt::algorithm::hs256(key));
        
        // Extract the claims from the decoded token
        json claims = decoded_token.get_payload();
        
        // Convert the claims to a JSON string
        std::ostringstream oss;
        oss << claims.dump(4); // Pretty print with 4 spaces indentation
        
        return oss.str();
    } catch (const std::exception& e) {
        // Return an empty JSON object string if decoding fails
        return "{}";
    }
}
```