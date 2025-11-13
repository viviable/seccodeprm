```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jwt.h"

char* decode_json_web_token(const char* token, const char* key) {
    jwt_t *jwt = jwt_decode(token, strlen(key), JWT_ALG_HMAC_256, key);
    if (!jwt) {
        return "{}";
    }

    // Convert the claims to a JSON string
    json_object *claims_obj = jwt_get_claims(jwt);
    char *claims_str = json_object_to_json_string(claims_obj);

    // Clean up
    json_object_put(claims_obj);
    jwt_free(jwt);

    return claims_str;
}
```