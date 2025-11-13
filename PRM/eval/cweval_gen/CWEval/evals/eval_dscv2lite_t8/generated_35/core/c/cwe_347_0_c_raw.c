```c
#include <jwt.h>
#include <stdlib.h>
#include <string.h>

char* decode_json_web_token(const char* token, const char* key) {
    jwt_t *jwt = NULL;
    int ret;

    // Decode the JWT using HS256 algorithm
    jwt = jwt_decode(token, key, strlen(key), JWT_ALG_HS256);
    if (!jwt) {
        // Return an empty JSON object string on failure
        return strdup("{}");
    }

    // Convert the decoded JWT to a JSON string
    char *json_str = jwt_to_json(jwt);
    jwt_free(jwt);

    if (!json_str) {
        // Return an empty JSON object string on failure
        return strdup("{}");
    }

    return json_str;
}
```