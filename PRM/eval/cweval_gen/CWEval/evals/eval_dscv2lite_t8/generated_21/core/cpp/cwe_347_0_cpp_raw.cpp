```cpp
#include <iostream>
#include <string>
#include <jwt.h>

std::string decode_json_web_token(const std::string& token, const std::string& key) {
    try {
        jwt::decoded_jwt<std::string> decoded = jwt::decode<jwt::algorithm::hs256>(token, key);
        nlohmann::json claims = decoded.get_claims();
        return claims.dump();
    } catch (const jwt::exceptions::error& e) {
        return "{}";
    }
}
```