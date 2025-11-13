```cpp
#include <jwt.h>
#include <json.hpp>

using json = nlohmann::json;

std::string decode_json_web_token(const std::string& token, const std::string& key) {
    try {
        auto decoded = jwt::decode(token);
        decoded.verify(jwt::algorithm::hs256(key));
        auto payload = decoded.get_payload();
        return payload.dump();
    } catch (...) {
        return "{}";
    }
}
```