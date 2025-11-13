```cpp
#include <iostream>
#include <string>
#include <jwt.h>

std::string decode_json_web_token(const std::string& token, const std::string& key) {
    try {
        jwt::decoded_jwt<std::string> decoded = jwt::decode<std::string>(token);
        auto verifier = jwt::verify<>::with_hmac_sha256(key);
        verifier.verify(decoded);

        nlohmann::json claims = decoded.get_payload_claims();
        return claims.dump();
    } catch (...) {
        return "{}";
    }
}
```