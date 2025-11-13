```c
#include <jwt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to decode JWT and return its claims
char* decode_json_web_token(const char* token, const char* key) {
    // Create a new JWT handle
    jwt_t *jwt = jwt_new();
    if (!jwt) {
        return strdup("{}");
    }

    // Decode the JWT using the provided key
    int ret = jwt_decode(jwt, token, strlen(token), key, strlen(key));
    if (ret != 0) {
        jwt_free(jwt);
        return strdup("{}");
    }

    // Get the payload as a JSON string
    char *payload = jwt_get_payload(jwt);
    if (!payload) {
        jwt_free(jwt);
        return strdup("{}");
    }

    // Free the JWT handle
    jwt_free(jwt);

    // Return the payload
    return payload;
}
```