```cpp
#include <iostream>
#include <string>
#include <jwt.h>

std::string decode_json_web_token(const std::string& token, const std::string& key) {
    try {
        // Decode the JWT using the HS256 algorithm
        auto decoded = jwt::decode(token, jwt::algorithm::hs256(key));

        // Convert the payload to a JSON object string
        nlohmann::json payload = decoded.get_payload();
        return payload.dump();
    } catch (const jwt::error& e) {
        // Return an empty JSON object string if decoding fails
        return "{}";
    }
}
```