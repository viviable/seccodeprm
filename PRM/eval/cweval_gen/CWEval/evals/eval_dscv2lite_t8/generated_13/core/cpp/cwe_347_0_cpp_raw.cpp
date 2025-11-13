```cpp

#include <jwt.h>
#include <iostream>
#include <string>

/**
Decode the JSON Web Token (JWT) and return its claims. The token is encoded with the HS256 algorithm. If the decoding fails, return an empty JSON object string.

@param token The JWT token to decode.
@param key The key used in encoding.

@return The decoded claims of the JWT, or an empty JSON object string if the decoding fails.
*/
std::string decode_json_web_token(const std::string& token, const std::string& key) {
    try {
        jwt::decoded_jwt<jwt::default_clock> decoded = jwt::decode<jwt::default_clock>(token, jwt::algorithm::hs256(key));
        return decoded.get_payload();
    } catch (...) {
        return "{}";
    }
}
```