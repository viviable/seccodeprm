```c
#include <jwt.h>
#include <stdlib.h>
#include <string.h>

char* decode_json_web_token(const char* token, const char* key) {
    jwt_t* jwt = jwt_new();
    jwt_set_algorithm(jwt, JWT_ALG_HS256);
    jwt_set_key(jwt, key);

    int ret = jwt_decode(jwt, token);
    if (ret != 0) {
        jwt_free(jwt);
        return strdup("{}");
    }

    const char* payload = jwt_get_payload(jwt);
    jwt_free(jwt);

    // Return a copy of the payload since it might be freed after this function returns
    return strdup(payload);
}
```