```cpp

using namespace jwtpp;

std::string decode_json_web_token(const std::string& token, const std::string& key) {
    try {
        auto decoded = decode(token);
        decoded.verify<algorithms::hs256>(key);
        return decoded.get_payload();
    } catch (...) {
        return "{}";
    }
}
```