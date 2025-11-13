```c
#include <jwt.h>
#include <stdlib.h>
#include <string.h>

char* decode_json_web_token(const char* token, const char* key) {
    jwt_t* jwt = jwt_decode(token, strlen(key), JWT_ALG_HS256, key);
    if (!jwt) {
        return "{}";
    }

    json_object* claims = jwt_get_claims(jwt);
    if (!claims) {
        jwt_free(jwt);
        return "{}";
    }

    char* claims_str = json_object_to_json_string(claims);
    json_object_put(claims);
    jwt_free(jwt);

    return claims_str;
}
```