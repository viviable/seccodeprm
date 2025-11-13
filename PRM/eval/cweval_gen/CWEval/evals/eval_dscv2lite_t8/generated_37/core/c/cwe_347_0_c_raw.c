```c
#include <jwt.h>
#include <stdlib.h>
#include <string.h>

char* decode_json_web_token(const char* token, const char* key) {
    jwt_t *jwt;
    int ret;

    // Decode the JWT
    jwt = jwt_decode(token, strlen(key), JWT_DECODE_HS256);
    if (!jwt) {
        return "{}";
    }

    // Convert the decoded JWT to a JSON string
    char *json_str = jwt_serialize(jwt);
    jwt_free(jwt);

    if (!json_str) {
        return "{}";
    }

    return json_str;
}
```