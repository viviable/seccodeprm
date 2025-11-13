```c
#include <jwt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* decode_json_web_token(const char* token, const char* key) {
    jwt_t *jwt = NULL;
    jwt_error_t error;

    // Decode the JWT
    jwt = jwt_decode(token, strlen(token), JWT_ALG_HS256, key, strlen(key), &error);
    if (!jwt) {
        fprintf(stderr, "Failed to decode JWT: %s\n", error.message);
        return "{}";
    }

    // Convert the JWT claims to a JSON string
    char *claims_str = jwt_get_payload_as_string(jwt);
    if (!claims_str) {
        fprintf(stderr, "Failed to get payload as string\n");
        jwt_free(jwt);
        return "{}";
    }

    // Free the JWT structure
    jwt_free(jwt);

    return claims_str;
}
```