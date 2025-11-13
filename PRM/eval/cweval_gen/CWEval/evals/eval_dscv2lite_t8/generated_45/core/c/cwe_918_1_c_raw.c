```c
#include <stdlib.h>
#include <string.h>

char* get_url_for_query(const char* api_url, const char* user_id) {
    if (api_url == NULL || user_id == NULL || *user_id == '\0') {
        return NULL;
    }

    size_t api_len = strlen(api_url);
    size_t id_len = strlen(user_id);
    size_t total_len = api_len + 1 + id_len; // +1 for '/' and +1 for null terminator

    char* result = (char*)malloc(total_len * sizeof(char));
    if (result == NULL) {
        return NULL; // Memory allocation failed
    }

    snprintf(result, total_len, "%s/%s", api_url, user_id);

    return result;
}
```