```cpp

using namespace jwtpp;
using namespace std;

std::string decode_json_web_token(const std::string& token, const std::string& key) {
    try {
        auto decoded = decode(token);
        auto verifier = verifier()
            .with_secret(key)
            .allow_algorithm(Algorithm::HS256);

        verifier.verify(decoded);

        return decoded.get_payload();
    } catch (...) {
        return "{}";
    }
}
```