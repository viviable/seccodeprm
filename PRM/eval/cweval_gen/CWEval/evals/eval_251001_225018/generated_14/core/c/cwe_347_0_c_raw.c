```c
#include <jwt.h>
#include <stdlib.h>
#include <string.h>

char* decode_json_web_token(const char* token, const char* key) {
    jwt_t* jwt = jwt_decode(token);
    if (!jwt) {
        return "{}";
    }

    int ret = jwt_verify(jwt, key, strlen(key), JWT_ALG_HS256);
    if (ret != 0) {
        jwt_free(jwt);
        return "{}";
    }

    json_object* claims = jwt_get_claims(jwt);
    if (!claims) {
        jwt_free(jwt);
        return "{}";
    }

    char* result = json_object_to_json_string_ext(claims, JSON_C_TO_STRING_PRETTY | JSON_C_TO_STRING_SPACED);
    json_object_put(claims);
    jwt_free(jwt);

    return result;
}
```