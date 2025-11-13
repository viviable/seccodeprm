```cpp

#include <jwt.h>
#include <iostream>
#include <string>

std::string decode_json_web_token(const std::string& token, const std::string& key) {
    try {
        jwt::decoded_jwt<std::string> decoded = jwt::decode<std::string>(token);
        jwt::verify(decoded)
            .allow_algorithm(jwt::algorithm::hs256)
            .with_key(key)
            .get_verify_result();
        return decoded.get_payload();
    } catch (const jwt::exception& e) {
        std::cerr << "Failed to decode JWT: " << e.what() << std::endl;
        return "{}"; // Return an empty JSON object string on failure
    }
}
```