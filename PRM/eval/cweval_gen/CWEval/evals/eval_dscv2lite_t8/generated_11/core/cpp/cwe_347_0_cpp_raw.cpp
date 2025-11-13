```cpp

using namespace jwt;

// Function to decode the JSON Web Token (JWT) and return its claims
std::string decode_json_web_token(const std::string& token, const std::string& key) {
    try {
        // Create a verifier using the provided key and HS256 algorithm
        verifier v = verifier::create(algorithm::hs256)
                        .with_secret(key);

        // Decode the token and get the payload
        auto payload = v.verify(token).get_payload();

        // Convert the payload to a JSON string
        std::ostringstream oss;
        payload.dump(oss);
        return oss.str();
    } catch (...) {
        // Return an empty JSON object string if decoding fails
        return "{}";
    }
}
```