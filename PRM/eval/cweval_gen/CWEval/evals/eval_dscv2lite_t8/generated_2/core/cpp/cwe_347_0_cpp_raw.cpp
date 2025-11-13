```cpp
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

extern "C" {
    #include <jwt.h>
}

using json = nlohmann::json;

std::string decode_json_web_token(const std::string& token, const std::string& key) {
    jwt_t *jwt;
    int ret;

    // Decode the JWT
    jwt = jwt_decode(token.c_str());
    if (!jwt) {
        return "{}";
    }

    // Verify the JWT using the provided key
    ret = jwt_verify(jwt, JWT_ALG_HS256, key.c_str(), key.size());
    if (ret != 0) {
        jwt_free(jwt);
        return "{}";
    }

    // Extract the payload (claims)
    json claims;
    try {
        claims = json::parse(jwt_get_payload(jwt));
    } catch (...) {
        jwt_free(jwt);
        return "{}";
    }

    // Clean up
    jwt_free(jwt);

    // Return the claims as a JSON string
    return claims.dump();
}
```