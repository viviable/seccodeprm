```c
#include <jwt.h>
#include <stdlib.h>
#include <string.h>

char* decode_json_web_token(const char* token, const char* key) {
    // Decode the JWT using the provided key
    jwt_t* jwt = jwt_decode(token, key);
    if (!jwt) {
        // Return an empty JSON object string if decoding fails
        return strdup("{}");
    }

    // Convert the JWT payload to a JSON string
    char* claims = jwt_serialize(jwt->payload);
    jwt_free(jwt);

    return claims;
}
```