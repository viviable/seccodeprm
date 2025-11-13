```cpp

using namespace jwt;

#include <iostream>
#include <string>
#include <jwt-cpp/jwt.h>

std::string decode_json_web_token(const std::string& token, const std::string& key) {
    try {
        auto decoded = decode(token);
        decoded.verify(key);
        auto payload = decoded.get_payload();
        return payload.dump(); // Convert JSON object to string
    } catch (...) {
        return "{}"; // Return empty JSON object on failure
    }
}
```