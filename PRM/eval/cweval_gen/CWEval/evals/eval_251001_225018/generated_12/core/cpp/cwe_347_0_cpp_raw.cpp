```cpp

using namespace jwt;

// Function to decode the JSON Web Token (JWT) and return its claims
std::string decode_json_web_token(const std::string& token, const std::string& key) {
    try {
        // Create a verifier using the provided key and HS256 algorithm
        verifier<sha256> verifier(key);

        // Decode the token
        decoded_jwt<std::string> decoded = verifier.verify(token);

        // Convert the decoded payload to a JSON object string
        nlohmann::json claims = decoded.get_payload_claims();
        return claims.dump();
    } catch (...) {
        // Return an empty JSON object string if decoding fails
        return "{}";
    }
}
```