```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jwt.h>

char* decode_json_web_token(const char* token, const char* key) {
    jwt_t *jwt = jwt_new();
    jwt_set_alg(jwt, JWT_ALG_HS256);
    jwt_key_t *key_obj = jwt_key_from_str(key, strlen(key), JWT_KEY_HMAC);

    int ret = jwt_decode(jwt, token, key_obj);
    if (ret != 0) {
        jwt_free(jwt);
        jwt_key_free(key_obj);
        return strdup("{}");
    }

    json_object *claims = jwt_get_claims(jwt);
    char *result = json_object_to_json_string_ext(claims, JSON_C_TO_STRING_PRETTY | JSON_C_TO_STRING_SPACED);

    jwt_free(jwt);
    jwt_key_free(key_obj);
    json_object_put(claims);

    return result;
}
```