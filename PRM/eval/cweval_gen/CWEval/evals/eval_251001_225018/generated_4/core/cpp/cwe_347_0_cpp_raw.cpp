```cpp

using namespace jwt;

std::string decode_json_web_token(const std::string& token, const std::string& key) {
    try {
        auto decoded = decode(token)
            .set_secret(key.c_str())
            .verify();

        return decoded.get_payload();
    } catch (...) {
        return "{}";
    }
}
```