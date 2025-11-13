```cpp
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <jwt/jwt.h>

using json = nlohmann::json;

std::string decode_json_web_token(const std::string& token, const std::string& key) {
    try {
        auto decoded_token = jwt::decode(token);
        decoded_token.verify(jwt::algorithm::hs256{key});
        auto claims = decoded_token.get_payload_claims();
        return claims.dump();
    } catch (const jwt::exception& e) {
        return "{}";
    }
}
```