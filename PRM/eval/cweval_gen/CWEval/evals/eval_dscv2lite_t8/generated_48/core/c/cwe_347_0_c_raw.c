```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

#define JWT_SECRET_KEY "your_secret_key_here"

char* decode_json_web_token(const char* token, const char* key) {
    // Function implementation goes here
}
```