```cpp

#include <jwt.h>
#include <json/json.h>
#include <iostream>

std::string decode_json_web_token(const std::string& token, const std::string& key) {
    try {
        // Decode the JWT using HS256 algorithm
        jwt::decoded_jwt<jwt::default_clock> decoded = jwt::decode<jwt::default_clock>(token);
        decoded.verify(jwt::algorithm::hs256(key));

        // Extract the claims from the decoded JWT
        Json::Value claims;
        for (const auto& claim : decoded.get_claims()) {
            claims[claim.first] = claim.second.to_string();
        }

        // Convert the claims to JSON string
        Json::StreamWriterBuilder builder;
        builder["commentStyle"] = "None";
        builder["indentation"] = "";
        return Json::writeString(builder, claims);
    } catch (...) {
        // Return an empty JSON object string if decoding fails
        return "{}";
    }
}
```