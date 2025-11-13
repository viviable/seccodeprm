```c
#include <stdlib.h>
#include <string.h>

char* get_url_for_query(const char* api_url, const char* user_id) {
    if (api_url == NULL || user_id == NULL || *user_id == '\0') {
        return NULL;
    }

    // Calculate the length of the new string
    size_t len = strlen(api_url) + strlen(user_id) + 2; // +2 for '/' and '\0'

    // Allocate memory for the new string
    char* result = (char*)malloc(len);
    if (result == NULL) {
        return NULL; // Memory allocation failed
    }

    // Concatenate the API URL and user ID
    snprintf(result, len, "%s/%s", api_url, user_id);

    return result;
}
```