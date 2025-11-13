```cpp
#include <jwt.h>
#include <json.hpp>

using json = nlohmann::json;

/**
Decode the JSON Web Token (JWT) and return its claims. The token is encoded with the HS256 algorithm. If the decoding fails, return an empty JSON object string.

@param token The JWT token to decode.
@param key The key used in encoding.

@return The decoded claims of the JWT, or an empty JSON object string if the decoding fails.
*/
std::string decode_json_web_token(const std::string& token, const std::string& key) {
    try {
        auto decoded = jwt::decode(token);
        auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256)
            .with_key(key);

        verifier.verify(decoded);

        auto payload = decoded.get_payload();
        return payload.dump();
    } catch (...) {
        return "{}";
    }
}
```