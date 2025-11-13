```cpp

using namespace jwt;

#include <iostream>
#include <string>
#include <stdexcept>

// Function to decode JWT and return its claims
std::string decode_json_web_token(const std::string& token, const std::string& key) {
    try {
        // Decode the JWT using HS256 algorithm
        auto decoded = decode(token)
            .set_secret(key)
            .verify();

        // Return the payload as a JSON string
        return decoded.get_payload();
    } catch (const std::exception& e) {
        // Return an empty JSON object string if decoding fails
        return "{}";
    }
}
```
This code snippet includes the necessary headers and implements the `decode_json_web_token` function according to the provided docstring. It uses the `jwt` library to decode the JWT and returns the claims as a JSON string. If the decoding fails, it catches the exception and returns an empty JSON object string.