```c
#include <jwt.h>
#include <stdlib.h>
#include <string.h>

char* decode_json_web_token(const char* token, const char* key) {
    jwt_t *jwt = NULL;
    char *payload = NULL;

    // Decode the JWT
    jwt = jwt_decode(token, strlen(token), key, strlen(key), "HS256");
    if (!jwt) {
        return "{}";
    }

    // Extract the payload
    payload = jwt_get_payload(jwt);
    if (!payload) {
        jwt_free(jwt);
        return "{}";
    }

    // Free the JWT structure
    jwt_free(jwt);

    return payload;
}
```