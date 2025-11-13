```c
#include <stdlib.h>
#include <string.h>

char* get_url_for_query(const char* api_url, const char* user_id) {
    if (api_url == NULL || user_id == NULL || *user_id == '\0') {
        return NULL;
    }

    size_t api_len = strlen(api_url);
    size_t user_len = strlen(user_id);
    size_t total_len = api_len + user_len + 2; // +1 for '/' and +1 for null terminator

    char* result = (char*)malloc(total_len);
    if (result == NULL) {
        return NULL;
    }

    snprintf(result, total_len, "%s/%s", api_url, user_id);
    return result;
}
```