```cpp
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <jwt-cpp/jwt.h>

using json = nlohmann::json;

std::string decode_json_web_token(const std::string& token, const std::string& key) {
    try {
        auto decoded = jwt::decode(token);
        auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256)
            .with_key(key);

        verifier.verify(decoded);

        return decoded.get_claims().dump();
    } catch (const std::exception& e) {
        return "{}";
    }
}
```