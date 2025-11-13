```c
#include <stdlib.h>
#include <string.h>

char* get_url_for_query(const char* api_url, const char* user_id) {
    if (api_url == NULL || user_id == NULL || *user_id == '\0') {
        return NULL;
    }

    // Calculate the length of the resulting URL
    size_t api_len = strlen(api_url);
    size_t user_len = strlen(user_id);
    size_t total_len = api_len + user_len + 2; // +1 for '/' and +1 for null terminator

    // Allocate memory for the resulting URL
    char* result_url = (char*)malloc(total_len);
    if (result_url == NULL) {
        return NULL; // Memory allocation failed
    }

    // Construct the URL
    snprintf(result_url, total_len, "%s/%s", api_url, user_id);

    return result_url;
}
```