```c
#include <stdlib.h>
#include <string.h>

char* get_url_for_query(const char* api_url, const char* user_id) {
    if (api_url == NULL || user_id == NULL || *user_id == '\0') {
        return NULL;
    }

    // Calculate the length of the new URL
    size_t len = strlen(api_url) + strlen(user_id) + 2; // +2 for '/' and '\0'

    // Allocate memory for the new URL
    char* url = (char*)malloc(len);
    if (url == NULL) {
        return NULL; // Memory allocation failed
    }

    // Construct the new URL
    snprintf(url, len, "%s/%s", api_url, user_id);

    return url;
}
```