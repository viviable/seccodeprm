```cpp

using namespace jwt;

std::string decode_json_web_token(const std::string& token, const std::string& key) {
    try {
        auto decoded = decode(token);
        auto verified = decoded.verify(algorithm::hs256(key));
        if (!verified) {
            throw std::runtime_error("Verification failed");
        }
        return decoded.get_payload();
    } catch (...) {
        return "{}";
    }
}
```