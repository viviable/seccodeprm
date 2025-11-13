```cpp

using namespace jwtpp;

std::string decode_json_web_token(const std::string& token, const std::string& key) {
    try {
        auto decoded = decode(token);
        decoded.verify<algorithms::hs256>(key);

        nlohmann::json claims = decoded.get_payload();
        return claims.dump();
    } catch (...) {
        return "{}";
    }
}
```